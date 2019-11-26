#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <math.h> 

#define SIZE 1024

volatile __uint64_t A[SIZE][SIZE];
volatile __uint64_t B[SIZE][SIZE];
volatile __uint64_t C[SIZE][SIZE];
volatile __uint64_t D[SIZE][SIZE];
// volatile __uint64_t E[SIZE][SIZE];


void init(volatile __uint64_t A[][SIZE], volatile __uint64_t B[][SIZE])
{
	int r, c;

	for (c = 0; c < SIZE; c++) {
		for (r = 0; r < SIZE; r++) {
			A[r][c] = rand();
			B[r][c] = rand();
		}
	}
}

int verify(volatile __uint64_t C[][SIZE], volatile __uint64_t D[][SIZE])
{
	int r, c;

	for (c = 0; c < SIZE; c++) {
		for (r = 0; r < SIZE; r++) {
			// printf("%llu, %llu\n", C[r][c], D[r][c]);
			if (C[r][c] != D [r][c]) {
				printf("error!\n");
				return -1;
			}
			
		}
	}
	return 0;
}

void matmul(volatile __uint64_t A[][SIZE], volatile __uint64_t B[][SIZE])
{
	int rowA, colB, idx;

	for (rowA = 0; rowA < SIZE; rowA++) {
		for (colB = 0; colB < SIZE; colB++) {
			for (idx = 0; idx < SIZE; idx++) {
				C[rowA][colB] += A[rowA][idx] * B[idx][colB];
			}
		}
	}
}

void transpose(volatile __uint64_t B[][SIZE])
{
	int row, col;
	__uint64_t temp;

	for (row = 0; row < SIZE; row++) {
		for (col = row+1; col < SIZE; col++) {
			temp = B[row][col]; // temporarily store value at B[row][col]
			B[row][col] = B[col][row]; // swap values across diaganol of matrix
			B[col][row] = temp;
		}
	}
}

// Modified matrix multiplication function to exploit spatial locality 
// of transposed B. Yields the same result of A*B but multiplies each row of 
// A with each row of B.
void matmulTranspose(volatile __uint64_t A[][SIZE], volatile __uint64_t B[][SIZE])
{
	int rowA, rowB, idx;

	for (rowA = 0; rowA < SIZE; rowA++) {
		for(rowB = 0; rowB < SIZE; rowB++) {
			for (idx = 0; idx < SIZE; idx++) {
				D[rowA][rowB] += A[rowA][idx] * B[rowB][idx];
			}
		}
	}
}


// Modified version of matrix multiplication that uses a tiling approach to 
// exploit temporal locality. Takes tileSize as an argument to build tiles 
// that are tileSize x tileSize large. 
void matmulTile(volatile __uint64_t A[][SIZE], volatile __uint64_t B[][SIZE], int tileSize)
{
	int i, j, k, J, K;

	for(k = 0; k < SIZE; k+=tileSize) // iterate over tile row
	{
		for(j = 0; j < SIZE; j+=tileSize) // iterate over tile column
		{
			for(i = 0; i < SIZE; i++) // iterate over entire matrix row
			{
				for(J = j; J < (((j+tileSize) < SIZE)?(j+tileSize):SIZE); J++) // adjust param to iterate over each row in each tile
				{
					for(K = k; K < (((k+tileSize) < SIZE)?(k+tileSize):SIZE); K++) // adjust param to iterate over each col in each tile
					{
						D[i][J] += A[i][K] * B[K][J];
					}
				}
			}
		}
	}
}

int main(int argc, char **argv)
{
	clock_t t;
	double time_taken;

	init(A, B);
	memset((__uint64_t**)C, 0, sizeof(__uint64_t) * SIZE * SIZE);
	memset((__uint64_t**)D, 0, sizeof(__uint64_t) * SIZE * SIZE);

	t = clock();
	matmul(A, B);
	t = clock() - t;
	time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

	printf("Matmul took %f seconds to execute \n", time_taken);

	for(int i = 0;i<=10;i++)
	{
		int b = pow(2,i); // b = tile size
		// reset D to all 0's to verify each instance of matmulTile produces
		// correct output
		memset((__uint64_t**)D, 0, sizeof(__uint64_t) * SIZE * SIZE);
		t = clock();
		matmulTile(A, B, b);
		t = clock() - t;
		time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

		verify(C,D); // check D is correct (prints "error!" if wrong)

		printf("MatmulTile with %d tiles took %f seconds to execute \n", b, time_taken);
	}


	// reset D
	memset((__uint64_t**)D, 0, sizeof(__uint64_t) * SIZE * SIZE);
	transpose(B);
	
	t = clock();
	matmulTranspose(A,B);
	t = clock() - t;
	time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

	verify(C,D); // check D is correct (prints "error!" if wrong)

	printf("matmulTranspose took %f seconds to execute \n", time_taken);
}












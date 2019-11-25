#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 1024

volatile __uint64_t A[SIZE][SIZE];
volatile __uint64_t B[SIZE][SIZE];
volatile __uint64_t C[SIZE][SIZE];
volatile __uint64_t D[SIZE][SIZE];


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
				printf("%d, %d\n", r, c);
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
	int row, col, idx;
	__uint64_t temp;

	for (row = 0; row < SIZE; row++) {
		for (col = row+1; col < SIZE; col++) {
			B[col][row] = B[row][col];
			temp = B[row][col];
			B[row][col] = B[col][row];
			B[col][row] = temp;
		}
	}
}

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
	
	transpose(B);
	// matmulTranspose(A,B);
	// verify(C,D);
	
	t = clock();
	matmulTranspose(A,B);
	t = clock() - t;
	time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

	verify(C,D);

	printf("matmulTranspose took %f seconds to execute \n", time_taken);
}












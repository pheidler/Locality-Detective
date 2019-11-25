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
			if (C[r][c] != D [r][c]) {
				printf("error!\n");
				printf("%d, %d", r, c);
				return -1;
			}
			
		}
	}
	return 0;
}

void matmul(volatile __uint64_t A[][SIZE], volatile __uint64_t B[][SIZE])
{
	int acceses = 0;
	int rowA, colB, idx;

	for (rowA = 0; rowA < SIZE; rowA++) {
		for (colB = 0; colB < SIZE; colB++) {
			for (idx = 0; idx < SIZE; idx++) {
				acceses++;
				C[rowA][colB] += A[rowA][idx] * B[idx][colB];
			}
		}
	}
	printf("Accesses: %i", acceses);
}

void transpose(volatile __uint64_t B[][SIZE])
{
	int row, col, idx;

	for (row = 0; row < SIZE-1; row++) {
		for (col = row+1; col < SIZE; col++) {
			B[row][col] = B[col][row];
		}
	}
}

void matmulTranspose(volatile __uint64_t A[][SIZE], volatile __uint64_t B[][SIZE])
{
	int rowA, rowB, idx;

	for (rowA = 0; rowA < SIZE; rowA++) {
		for(rowB = 0; rowB < SIZE; rowB++) {
			for (idx = 0; idx < SIZE; idx++) {
				D[rowA][rowB] += A[rowA][idx] * B[rowA][idx];
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
	
	// transpose(B);

	t = clock();
	matmul(A, B);
	// matmulTranspose(A,B);
	t = clock() - t;
	time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

	transpose(B);
	matmulTranspose(A,B);
	verify(C,D);
	
	printf("Matmul took %f seconds to execute \n", time_taken);
}












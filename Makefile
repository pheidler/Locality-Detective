runMatrixMult: MatrixMult.o 
	gcc -o runMatrixMult MatrixMult.o 

MatrixMult.o: MatrixMult.c
	gcc -c -std=c99 -Wall MatrixMult.c
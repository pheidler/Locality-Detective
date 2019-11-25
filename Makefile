runMatrixMult: MatrixMult.o 
	gcc -o runMatrixMult MatrixMult.o 
	./runMatrixMult

MatrixMult.o: MatrixMult.c
	gcc -c -std=c99 -Wall MatrixMult.c

push: 
	git add .
	git commit -m "commiting"
	git push
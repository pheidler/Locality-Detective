#CSE 120 HA5 Makefile

#  make                     makes MatrixMult
#  make clean               removes binaries
#  push			    push changes to git repo

runMatrixMult: MatrixMult.o 
	gcc -o runMatrixMult MatrixMult.o -lm
	./runMatrixMult

MatrixMult.o: MatrixMult.c
	gcc -c -std=c99 -Wall MatrixMult.c 

clean :
	rm -f runMatrixMult matrixMult.o

push: 
	git add .
	git commit -m "commit"
	git push
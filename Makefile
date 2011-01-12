reduce: reduce.o
	mpicc -g -Wall -o reduce reduce.o -lssl

reduce.o: reduce.c
	mpicc -g -Wall -c reduce.c 

#include <stdio.h>
#include <mpi.h>
#include <openssl/md5.h>

int LENGTH=7;

void print_hello(int argc, char *argv[]){
    int rank, size;
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    if (rank<argc)
        printf("[%d.%d]: Argv %s\n", rank, size, argv[rank+1]);
    printf("\n");
}

int main(int argc, char *argv[])
{
    char argv[LENGTH];
    scanf(&argv);
    MPI_Init (&argc, &argv);
    print_hello(argc, argv);
    MPI_Finalize();
    return 0;
}

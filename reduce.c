#include <stdio.h>
#include <mpi.h>
#include <openssl/md5.h>

void check_param(int arg){
    if (arg<2) {
        printf("Error, no argument supplied\n");
        exit(1);
    } else {
        printf("Params OK. Key %d passed\n", arg);
    }
}

void print_hello(){
    int rank, size;
    (void)MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    (void)MPI_Comm_size (MPI_COMM_WORLD, &size);
    (void)printf ("[%d.%d]: Hello, world!\n",
                    rank, size);
}

int main(int argc, char *argv[])
{
    char *filename;

    check_param(argc);

    (void)MPI_Init (&argc, &argv);
    print_hello();
    (void)MPI_Finalize();
    return 0;
}

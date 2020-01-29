#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int n=20000;
    int number1[n]={0};
    int number2[n]={0};
    for(int i=0;i<n;i++){
        number1[i]=rand()%10;
        number2[i]=rand()%10;
    }
    if (world_rank == 0) {  
        MPI_Send(&number1, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&number1, n, MPI_INT, 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 0 received number %d from process 1\n",
               number1[0]);
    } else if (world_rank == 1) {
        MPI_Send(&number2, n, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&number2, n, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n",
               number2[0]);        
    }

    // Finalize the MPI environment.
    MPI_Finalize();
}
// MPI_Send(
//     void* data,
//     int count,
//     MPI_Datatype datatype,
//     int destination,
//     int tag,
//     MPI_Comm communicator)
// MPI_Recv(
//     void* data,
//     int count,
//     MPI_Datatype datatype,
//     int source,
//     int tag,
//     MPI_Comm communicator,
//     MPI_Status* status)
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == 0) {
        int number;
        number = 3;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 0 received number %d from process 1\n",
               number);
    } else if (world_rank == 1) {
        int number;
        MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n",
               number);
        number=4;
        MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        
    }

    // Finalize the MPI environment.
    MPI_Finalize();
}
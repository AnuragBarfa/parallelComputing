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
        int n=24;
        int p=world_size;
        int number[n]={0};
        for(int i=0;i<n;i++)number[i]=rand()%100;
        int check=0;
        for(int i=0;i<n;i++)check+=number[i];
        printf("%d\n", check);
        int arraySizeToSend=n/p;
        int arraySum=0;
        //giveing different tag to message allow to send two message simultaneous without reading any one 
        for(int i=1;i<p;i++){
            // sending array size to all other process
            MPI_Send(&arraySizeToSend, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            
            //sending array to other process
            MPI_Send(number+i*arraySizeToSend, arraySizeToSend, MPI_INT, i, 1, MPI_COMM_WORLD);
            int partialSum=0;
            printf("both message sent\n"); 
            //recieving partial sum
            MPI_Recv(&partialSum, 1, MPI_INT, i, 2, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            
            printf("Received partial sum %d from process %d\n", partialSum, i);
            arraySum+=partialSum;    
        }    
        for(int j=0;j<arraySizeToSend;j++)arraySum+=number[j];
        printf("Sum of array: %d\n", arraySum);
    } 
    else{
        int arraySizeToRecieve;
        
        //recieving array size from P0
        MPI_Recv(&arraySizeToRecieve, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received array size %d from process 0\n",
               world_rank,arraySizeToRecieve);
        int receivedNumbers[arraySizeToRecieve];
        //recieving array from P0
        MPI_Recv(receivedNumbers, arraySizeToRecieve, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int i=0;i<arraySizeToRecieve;i++)printf("%d ", receivedNumbers[i]);
        printf("\n");
        int partialSum=0;
        for(int i=0;i<arraySizeToRecieve;i++)partialSum+=receivedNumbers[i];
        
        // sending partial sum back to P0
        MPI_Send(&partialSum, 1, MPI_INT, 0, 2 , MPI_COMM_WORLD);
    }

    // Finalize the MPI environment.
    MPI_Finalize();
}
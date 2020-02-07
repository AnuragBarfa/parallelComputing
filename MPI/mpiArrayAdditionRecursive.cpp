#include <mpi.h>
#include <stdio.h>
#include <iostream>
using namespace std;
int main(int argc, char** argv) {
    // Initialize the MPI environment
    // freopen("recursiveOutput.txt","a",stdout);
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int p=world_size;
    int *number;
    int n=64;//initially it should have been with P0 only and then distributed but for modified the idea  
    // int n=stoi(argv[1]);
    int start=-1;
    int end=-1;
    int arraySize=-1;
    int parentID =-1;
    if (world_rank == 0) {
        printf("Array size: %d\n", n);
        number=new int[n];
        for(int i=0;i<n;i++)number[i]=rand()%100;
        // int check=0;
        // for(int i=0;i<n;i++)check+=number[i];
        // printf("%d\n", check);
        start=0;
        end=p-1;
        arraySize=n;
    } 
    else{
        // printf("recieving start %d\n", world_rank);
        MPI_Status status;
        MPI_Recv(&start, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        parentID=status.MPI_SOURCE;
        MPI_Recv(&end, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&arraySize, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // printf("recieved by %d %d %d\n",world_rank,start,end);
        // printf("recieving done %d\n", world_rank);
        number=new int[arraySize];
        MPI_Recv(number, arraySize, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        // MPI_Send(number+destination, destination-start, MPI_INT, destination, 1, MPI_COMM_WORLD);    
    }
    int tempEnd=end;
    while(tempEnd>start){
        int destination=(tempEnd+start+1)/2;
        int arraySizeToSend=arraySize/2;
        // printf("sending start %d to %d\n", world_rank,destination);
        MPI_Send(&destination, 1, MPI_INT, destination, 0, MPI_COMM_WORLD);    
        MPI_Send(&tempEnd, 1, MPI_INT, destination, 0, MPI_COMM_WORLD);    
        MPI_Send(&arraySizeToSend, 1, MPI_INT, destination, 0, MPI_COMM_WORLD);
        MPI_Send(number+arraySize/2, arraySizeToSend, MPI_INT, destination, 0, MPI_COMM_WORLD);
        tempEnd=destination-1;
        arraySize/=2;
        // printf("sending done %d to %d\n", world_rank,destination);
    }
    // printf("number with process %d :\n", world_rank);
    // for(int i=0;i<arraySize;i++)printf("%d ", number[i]);
    // printf("\n");
    int partialSum=0;
    for(int i=0;i<arraySize;i++)partialSum+=number[i];
    while(){
        MPI_Recv();
    }
    if(world_rank!=0){
        MPI_Send(&partialSum, 1, MPI_INT, parentID, 0, MPI_COMM_WORLD);        
    }    
    double toc=MPI_Wtime(); 
// printf("Sum of array: %d\n", arraySum);
// cout<<toc-tic<<"\n";
// printf("Time required for computation(sec): %e\n", toc-tic);
    // Finalize the MPI environment.
    MPI_Finalize();
}
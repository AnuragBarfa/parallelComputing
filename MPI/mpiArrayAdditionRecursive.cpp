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
    // int n=64;//initially it should have been with P0 only and then distributed but for modified the idea  
    int n=stoi(argv[1]);
    int start=-1;
    int end=-1;
    int arraySize=-1;
    int parentID =-1;
    if (world_rank == 0) {
        printf("Array size: %d\n", n);
        
        //initializing data with process P0
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
        MPI_Status status;
        //Receving bound for which thic process is highest parent 
        MPI_Recv(&start, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        parentID=status.MPI_SOURCE;
        MPI_Recv(&end, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        //Receving size of array to receive and array also
        MPI_Recv(&arraySize, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        number=new int[arraySize];
        MPI_Recv(number, arraySize, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
    }
    //after process having respecting data it come to this point and start distributing based on bound 
    int tempEnd=end;
    while(tempEnd>start){
        int destination=(tempEnd+start+1)/2;
        int arraySizeToSend=arraySize/2;

        //Receving bound for which destination process is highest parent 
        MPI_Send(&destination, 1, MPI_INT, destination, 0, MPI_COMM_WORLD);    
        MPI_Send(&tempEnd, 1, MPI_INT, destination, 0, MPI_COMM_WORLD);    
        
        //Sending size of array to receive by destination process and array also
        MPI_Send(&arraySizeToSend, 1, MPI_INT, destination, 0, MPI_COMM_WORLD);
        MPI_Send(number+arraySize/2, arraySizeToSend, MPI_INT, destination, 0, MPI_COMM_WORLD);
        tempEnd=destination-1;
        arraySize/=2;//updating size of uvailable array after every round of distribution 
    }

    double tic=MPI_Wtime(); 
    
    // after  completion of distribution each processor adds available number with it
    int partialSum=0;
    for(int i=0;i<arraySize;i++)partialSum+=number[i];

    //recursivly gather all the partial sum values from the destination process where it has previously given data
    while(tempEnd!=end){
        int tempSum=0;
        tempEnd=start+2*(tempEnd-start)+1;
        int source=(tempEnd+start+1)/2;
        MPI_Recv(&tempSum, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        partialSum+=tempSum;
    }
    double toc=MPI_Wtime(); 
    if(world_rank!=0){
        MPI_Send(&partialSum, 1, MPI_INT, parentID, 0, MPI_COMM_WORLD);        
    }    
    else{
        printf("Sum of array: %d\n", partialSum);
        printf("Time required for computation(sec): %e\n", toc-tic);
    }
    
    
// cout<<toc-tic<<"\n";
    
    // Finalize the MPI environment.
    MPI_Finalize();
}
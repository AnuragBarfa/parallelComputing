#include <mpi.h>
#include <stdio.h>
#include <iostream>
using namespace std;
int main(int argc, char** argv) {
    // Initialize the MPI environment
    // freopen("naiveOutput.txt","a",stdout);
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // int n;
    int n=stoi(argv[1]);
    int p=world_size;
    int vectorSize=n/p;
    int *b;
    int **A;
    if (world_rank == 0) {
        b=new int[n];
        for(int i=0;i<n;i++)b[i]=rand()%100;
        // for(int i=0;i<n;i++)cout<<b[i]<<" ";
        // cout<<"\n";
        A=new int*[n];
        for(int i=0;i<n;i++){
            A[i]= new int[n];
            for(int j=0;j<n;j++)A[i][j]=rand()%100;
        }
        // MPI_Scatter(b, vectorSize, MPI_INT, b, vectorSize, MPI_INT, 0, MPI_COMM_WORLD);
        for(int i=1;i<p;i++){
            MPI_Send(&(A[0][0]), n*vectorSize, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        // MPI_Scatter(&(A[0][0]), n*vectorSize, MPI_INT, &(A[0][0]), n*vectorSize, MPI_INT, 0, MPI_COMM_WORLD);
    } 
    else{
        b=new int[vectorSize];
        A=new int*[vectorSize];
        for(int i=0;i<vectorSize;i++)A[i]=new int[n];
        // MPI_Scatter(NULL, vectorSize, MPI_INT, b, vectorSize, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Recv(&(A[0][0]), n*vectorSize, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        // MPI_Scatter(NULL, n*vectorSize, MPI_INT, &(A[0][0]), n*vectorSize, MPI_INT, 0, MPI_COMM_WORLD);
    }
    if(world_rank==0){
    //     printf("data with process %d\n", world_rank);
    // // for(int i=0;i<vectorSize;i++)cout<<b[i]<<" ";
    // // cout<<"\n";
    //     for(int i=0;i<vectorSize;i++){
    //     for(int j=0;j<n;j++)cout<<A[i][j]<<" ";
    //     cout<<"\n";
    // }
    }
    printf("data with process %d\n", world_rank);
    // for(int i=0;i<vectorSize;i++)cout<<b[i]<<" ";
    // cout<<"\n";
        for(int i=0;i<vectorSize;i++){
        for(int j=0;j<n;j++)cout<<A[i][j]<<" ";
        cout<<"\n";
    }
    for(int i=0;(i<vectorSize&&world_rank!=0)||(i<n&&world_rank==0);i++)delete[] A[i];
    delete[] A;
    // Finalize the MPI environment.
    MPI_Finalize();
}
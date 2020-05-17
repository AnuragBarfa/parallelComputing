#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char** argv) {
    // Initialize the MPI environment
    freopen("block.txt","a",stdout);
    // cout<<time(0);
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    srand(world_rank);
    int n=stoi(argv[1]);
    int p=world_size;
    int rows = n / sqrt(p);
    int cols = n / sqrt(p);
    int A[rows][cols];
    int b[rows];
    if(world_rank==0){
        printf("parameters used n = %d and no of threads = %d\n", n, p);
    }
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++)A[i][j]=rand()%10;
        b[i]=-1;
    }
    // printf("%s\n", "done with A");
    // color control of subset assignment (nonnegative integer). Processes with the same color are in the same new communicator
    int rowColor=world_rank/sqrt(p);
    int colColor=world_rank%(int) sqrt(p);
    MPI_Comm rowComm, colComm;

    MPI_Comm_split( MPI_COMM_WORLD, rowColor, world_rank, &rowComm );
    MPI_Comm_split( MPI_COMM_WORLD, colColor, world_rank, &colComm );

    int row_comm_size, col_comm_size;
    int row_comm_rank, col_comm_rank;

    MPI_Comm_size ( rowComm, &row_comm_size );
    MPI_Comm_size ( colComm, &col_comm_size );

    MPI_Comm_rank ( rowComm, &row_comm_rank );
    MPI_Comm_rank ( colComm, &col_comm_rank );
    // cout<<world_rank<<" "<<row_comm_rank<<" "<<col_comm_rank<<"\n";

    //intializing b with diagonal entries
    if(row_comm_rank==col_comm_rank){
        for(int i=0;i<rows;i++){
            b[i]=(rand())%100;
        }
    }
    // printf("%s\n", "done with b");
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();
    double minStart;
    //diagonal entry broadcast its data in it's column
    MPI_Bcast(b,rows,MPI_INT,row_comm_rank,colComm);
    // if(row_comm_rank==0){
    //     for(int i=0;i<rows;i++)cout<<b[i]<<" ";
    //     cout<<"\n";    
    // }
    // printf("%s\n", "done with broadcast");
    int partialRowResult[rows];
    for(int i=0;i<rows;i++){
        partialRowResult[i]=0;
        for(int j=0;j<cols;j++){
            partialRowResult[i]=partialRowResult[i]+A[i][j]*b[j];
        }
    }
    int rowResult[rows];
    MPI_Reduce(partialRowResult,rowResult,rows,MPI_INT,MPI_SUM,row_comm_rank,colComm);
    double end = MPI_Wtime();
    double maxEnd;

    MPI_Reduce(&start, &minStart, 1, MPI_DOUBLE,MPI_MIN, 0, MPI_COMM_WORLD); 
    MPI_Reduce(&end, &maxEnd, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if(world_rank==0) {
        double timeTaken = maxEnd - minStart;
        cout << "Time taken : "<< timeTaken << " seconds" << endl; 
    }
    MPI_Finalize();
}
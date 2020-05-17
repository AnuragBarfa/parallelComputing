#include <mpi.h>
#include <stdio.h>
#include <iostream>
using namespace std;
int main(int argc, char** argv) {
    // Initialize the MPI environment
    freopen("col.txt","a",stdout);
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int n=stoi(argv[1]);
    int p=world_size;
    int vectorSize=n/p;
    int b[vectorSize];
    int A[n][vectorSize];
    if(world_rank==0){
        printf("parameters used n = %d and no of threads = %d\n", n, p);
    }
    for(int i=0;i<vectorSize;i++)b[i]=rand()%10;
    for(int i=0;i<n;i++){
        for(int j=0;j<vectorSize;j++)A[i][j]=rand()%10;
    }
    // Assumed that data is available with each process no need for distribution
    int partialAns[n];
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();
    double minStart;
    for(int i=0;i<n;i++){
        partialAns[i]=0;
        for(int j=0;j<vectorSize;j++)partialAns[i]=A[i][j]*b[j];
    }
    int finalAns[n];
    MPI_Allreduce(partialAns,finalAns,n,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    // for(int i=0;i<n;i++)cout<<finalAns[i]<<" ";
    // cout<<"\n";
    double end = MPI_Wtime();
    double maxEnd;

    MPI_Reduce(&start, &minStart, 1, MPI_DOUBLE,MPI_MIN, 0, MPI_COMM_WORLD); 
    MPI_Reduce(&end, &maxEnd, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if(world_rank==0) {
        double timeTaken = maxEnd - minStart;
        cout << "Time taken : "<< timeTaken << " seconds" << endl; 
    }
    // Finalize the MPI environment.
    MPI_Finalize();
}


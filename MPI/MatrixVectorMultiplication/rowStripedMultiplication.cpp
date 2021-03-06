#include <mpi.h>
#include <stdio.h>
#include <iostream>
using namespace std;
int main(int argc, char** argv) {
    // Initialize the MPI environment
    freopen("row.txt","a",stdout);
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int n=stoi(argv[1]);
    int p=world_size;
    int vectorSize=n/p;
    int A[vectorSize][n];
    int partial_b[vectorSize];
    if(world_rank==0){
        printf("parameters used n = %d and no of threads = %d\n", n, p);
    }
    for(int i=0; i<vectorSize; i++) {
        for(int j=0; j<n; j++) A[i][j] = rand()%100;
    }
    for(int i=0; i<vectorSize; i++) partial_b[i] = rand()%100;
    // // int n;
    int b[n];
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();
    double minStart;
    MPI_Allgather(
        partial_b,               // data to be sent
        vectorSize,        // send count
        MPI_INT,         // type of data to be sent
        b,       // receive buffer
        vectorSize,        // receive count
        MPI_INT,         // type of data to be received
        MPI_COMM_WORLD   
    );
    for(int i=0; i<vectorSize; i++){
        partial_b[i] = 0;
        for(int j=0; j<n; j++){
            partial_b[i] += A[i][j]*b[j];
        }
    }
    double end = MPI_Wtime();
    double maxEnd;

    MPI_Reduce(&start, &minStart, 1, MPI_DOUBLE,MPI_MIN, 0, MPI_COMM_WORLD); 
    MPI_Reduce(&end, &maxEnd, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if(world_rank==0) {
        double timeTaken = maxEnd - minStart;
        cout << "Time taken : "<< timeTaken << " seconds" << endl; 
        // printf("%lf\n",timeTaken);
    }
    // Finalize the MPI environment.
    MPI_Finalize();
}

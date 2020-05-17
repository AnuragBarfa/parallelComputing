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
    int b[vectorSize];
    int A[vectorSize][n];
    if (world_rank == 0) {
        int bTemp[n];
        int ATemp[n][n];
        for(int i=0;i<n;i++)bTemp[i]=rand()%10;
        // for(int i=0;i<n;i++)cout<<b[i]<<" ";
        // cout<<"\n";
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++)ATemp[i][j]=rand()%10;
        }
        cout<<"A:\n";
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++)cout<<ATemp[i][j]<<" ";
            cout<<"\n";
        }
        cout<<"b:\n";
        for(int i=0;i<n;i++)cout<<bTemp[i]<<" ";
        cout<<"\n";
        int ans[n];
        for(int i=0;i<vectorSize;i++){
            ans[i]=0;
            for(int j=0;j<n;j++){
                ans[i]+=ATemp[i][j]*bTemp[j];
            }
        }
        for(int i=0;i<n;i++)cout<<ans[i]<<" ";
        cout<<"\n";
        MPI_Scatter(bTemp, vectorSize, MPI_INT, b, vectorSize, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(ATemp, n*vectorSize, MPI_INT, A, n*vectorSize, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else{
        MPI_Scatter(NULL, vectorSize, MPI_INT, b, vectorSize, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(NULL, n*vectorSize, MPI_INT, A, n*vectorSize, MPI_INT, 0, MPI_COMM_WORLD);
    }
    int b_hat[n];
    // MPI_Barrier(MPI_COMM_WORLD);
    MPI_Allgather(b, vectorSize, MPI_INT, b_hat, vectorSize, MPI_INT, MPI_COMM_WORLD);  
    printf("for process %d\n", world_rank);
    for(int i=0;i<n;i++)cout<<b_hat[i]<<" ";
        cout<<"\n";
    // for(int i=0;i<vectorSize;i++){
    //         for(int j=0;j<n;j++)cout<<A[i][j]<<" ";
    //             cout<<"\n";
    //     }
    // Finalize the MPI environment.
    MPI_Finalize();
}
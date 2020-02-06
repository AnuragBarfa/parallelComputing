#include <mpi.h>
#include <stdio.h>
#include <iostream>
using namespace std;
int main(int argc, char** argv) {
    // Initialize the MPI environment
    freopen("recursiveOutput.txt","a",stdout);
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    if (world_rank == 0) {
        int n=stoi(argv[1]);
        printf("Array size: %d\n", n);
        int p=world_size;
        int number[n]={0};
        for(int i=0;i<n;i++)number[i]=rand()%100;
        // int check=0;
        // for(int i=0;i<n;i++)check+=number[i];
        // printf("%d\n", check);
        int arraySizeToSend=n/p;
        
        double toc=MPI_Wtime(); 
        // printf("Sum of array: %d\n", arraySum);
        // cout<<toc-tic<<"\n";
        printf("Time required for computation(sec): %e\n", toc-tic);
    } 
    else{
        
    }

    // Finalize the MPI environment.
    MPI_Finalize();
}
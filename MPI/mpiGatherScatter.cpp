#include <mpi.h>
#include <stdio.h>
#include <iostream>
using namespace std;
int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int n=64;
    int *number;
    int arraySize=n/world_size;
    number=new int[arraySize];
    if (world_rank == 0) {
        int *rand_nums= new int[n];
        for(int i=0;i<n;i++)rand_nums[i]=rand()%100;
        // int check=0;
        // for(int i=0;i<n;i++)check+=rand_nums[i];
        // printf("%f\n", (float)check/n);
        // P0 distribute rand number to all process including itself
        MPI_Scatter(rand_nums, arraySize, MPI_INT, number,arraySize, MPI_INT, 0, MPI_COMM_WORLD);
    } 
    else{
        MPI_Scatter(NULL, arraySize, MPI_INT, number,arraySize, MPI_INT, 0, MPI_COMM_WORLD);
    }
    // printf("number with process %d \n", world_rank);
    // for(int i=0;i<arraySize;i++)cout<<number[i]<<" ";
    // cout<<"\n";
    float partialAverage=0;
    for(int i=0;i<arraySize;i++)partialAverage+=(float)number[i];
    partialAverage/=arraySize;
    float *partialAverages;
    if(world_rank==0){
        partialAverages= new float[world_size];
    }
    MPI_Gather(&partialAverage, 1, MPI_FLOAT, partialAverages, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if(world_rank==0){
        float ans=0;
        for(int i=0;i<world_size;i++)ans+=partialAverages[i];
        ans/=world_size;
        cout<<ans<<"\n";
    }
    // Finalize the MPI environment.
    MPI_Finalize();
}
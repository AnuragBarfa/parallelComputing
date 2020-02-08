#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char** argv) {
    // Initialize the MPI environment
    // freopen("naiveOutput.txt","a",stdout);
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int arraySize=-1;
    int n;
    int *number;
    int p=world_size;
    n=stoi(argv[1]);
    // n=8;
    int arraySizeToSend=n/p;
    if(world_rank==0){
        arraySize=n;
        number=new int[arraySize];
        for(int i=0;i<arraySize;i++)number[i]=rand()%100;
        for(int i=1;i<p;i++){
            MPI_Send(&arraySizeToSend, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

            MPI_Send(number+i*arraySizeToSend, arraySizeToSend, MPI_INT, i, 1, MPI_COMM_WORLD);    
        }
        arraySize=arraySizeToSend;
    }
    else{
        MPI_Recv(&arraySize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        number=new int[arraySize];
        MPI_Recv(number, arraySize, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    // locally sorting each block 
    sort(number, number + arraySizeToSend);
    int *number2=new int[arraySizeToSend];
    int *number3=new int[2*arraySizeToSend];
    for(int i=0;i<n;i++){
        // printf("round %d begin\n",i );
        if((i+world_rank)%2==0 && world_rank!=world_size-1){
            MPI_Recv(number2, arraySizeToSend, MPI_INT, world_rank+1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int ind1=0,ind2=0,ind3=0;
            while(ind1<arraySizeToSend||ind2<arraySizeToSend){
                if(ind1==arraySizeToSend){
                    number3[ind3]=number2[ind2];
                    ind2++;
                }
                else if(ind2==arraySizeToSend){
                    number3[ind3]=number[ind1];
                    ind1++;
                }
                else{
                    if(number[ind1]<number2[ind2]){
                       number3[ind3]=number[ind1];
                       ind1++; 
                    }
                    else{
                        number3[ind3]=number2[ind2];
                        ind2++;
                    }
                }
                ind3++;
            }
            for(int i1=0;i1<arraySizeToSend;i1++){
                number[i1]=number3[i1];
                number2[i1]=number3[i1+arraySizeToSend];
            }   
            MPI_Send(number2, arraySizeToSend, MPI_INT, world_rank+1, 1, MPI_COMM_WORLD);    

        }
        else if((i+world_rank)%2!=0 && world_rank!=0){
            MPI_Send(number, arraySizeToSend, MPI_INT, world_rank-1, 1, MPI_COMM_WORLD); 
            MPI_Recv(number, arraySizeToSend, MPI_INT, world_rank-1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    printf("number with process %d \n", world_rank);
    for(int i1=0;i1<arraySize;i1++)printf("%d ", number[i1]);
    printf("\n");
    // Finalize the MPI environment.
    MPI_Finalize();
}
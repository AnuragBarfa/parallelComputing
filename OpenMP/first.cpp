#include <stdio.h>
#include<omp.h>
int main(){
#pragma omp parallel for
  for(int i=0;i<10;i++){
    // printf("%i\n",i);
    int rank = omp_get_thread_num();
    printf("%d\n", rank);
  }
  return 0;
}
#include <stdio.h>
#include <pthread.h>
#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;

const int N=16;
int head=0;
int tail=0;
int myQueue[N];
pthread_mutex_t queue_mutex= PTHREAD_MUTEX_INITIALIZER;
void printArray(int *myQueue,int l, int h){
  for(int i=l;i<h;i++)printf("%d ", myQueue[i]);
    printf("\n");
}

void* producer(void* data){
  printf("prod\n");
  while(true){
    printf("in whi\n");
    pthread_mutex_lock(&queue_mutex);
    if(myQueue[tail]==0){
      myQueue[tail]=rand()%10+1;
      tail=(tail+1)%N;
    }
    else sleep(3);
    // printArray(myQueue,0,N);
    pthread_mutex_unlock(&queue_mutex);
  }
}
void* consumer(void* data){
  printf("cons\n");
  while(true){
    pthread_mutex_lock(&queue_mutex);
    if(myQueue[head]!=0){
      int x=myQueue[head];
      myQueue[head]=0;
      head=(head+1)%N;
      // printArray(myQueue,0,N);
      pthread_mutex_unlock(&queue_mutex);
      sleep(x);
    }
    else pthread_mutex_unlock(&queue_mutex);
  }
}
int main(){
  int p,c;
  p=2,c=2;
  for(int i=0;i<N;i++)myQueue[i]=0;
  pthread_t producer_threads[p];
  pthread_t consumer_threads[c];
  for(int i=0;i<p;i++){
    pthread_create(&producer_threads[i],NULL,producer,NULL);
  }
  for(int i=0;i<c;i++){
    pthread_create(&consumer_threads[i],NULL,consumer,NULL);
  }  
  for(int i=0;i<p;i++){
    pthread_join(producer_threads[i],NULL);
  }
  for(int i=0;i<c;i++){
    pthread_join(consumer_threads[i],NULL);
  }
  return 0;
}
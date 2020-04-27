#include <stdio.h>
#include <pthread.h>
#include<bits/stdc++.h>
#include<unistd.h>
#include <chrono> 
using namespace std;
using namespace std::chrono;

const int N=512;
int myQueue[N];
pthread_mutex_t queue_mutex[N];
int timeOfWork=0;
bool flag=false;
std::chrono::nanoseconds max_consumer_time;
std::chrono::nanoseconds total_consumer_time;
void printArray(int *myQueue,int l, int h){
  for(int i=l;i<h;i++)printf("%d ", myQueue[i]);
    printf("\n");
}

void* producer(void* data){
  while(!flag){
    for(int i=0;i<N;i++){
      pthread_mutex_lock(&queue_mutex[i]);
      if(myQueue[i]==0){
        myQueue[i]=rand()%10+1;
      }
      pthread_mutex_unlock(&queue_mutex[i]);
    }
    sleep(1);
    // printArray(myQueue,0,N);
  }
}
void* consumer(void* data){
  auto start = high_resolution_clock::now(); 
  while(!flag){
    for(int i=0;i<N;i++){
      pthread_mutex_lock(&queue_mutex[i]);
      if(myQueue[i]!=0){
        int x=myQueue[i];
        myQueue[i]=0;
        // printArray(myQueue,0,N);
        pthread_mutex_unlock(&queue_mutex[i]);
        timeOfWork+=x;
        sleep(x);
      }
      else pthread_mutex_unlock(&queue_mutex[i]);
    }
  }
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<nanoseconds>(stop - start); 
  if(duration>max_consumer_time)max_consumer_time=duration;
  // cout<<duration.count()<<endl;
  total_consumer_time+=duration;
}
int main(int argc ,char** argv){
  freopen("output2.txt","a",stdout);
  int p,c,sleepTime;
  p=stoi(argv[1]),c=stoi(argv[2]);sleepTime=stoi(argv[3]);
  cout << std::setprecision(20);
  auto start = high_resolution_clock::now(); 
  flag=false;max_consumer_time=total_consumer_time=duration_cast<nanoseconds>(start - start);
  for(int i=0;i<N;i++){
    myQueue[i]=0;
    queue_mutex[i]=PTHREAD_MUTEX_INITIALIZER;
  }
  timeOfWork=0;
  pthread_t producer_threads[p];
  pthread_t consumer_threads[c];
  for(int i=0;i<p;i++){
    pthread_create(&producer_threads[i],NULL,producer,NULL);
  }
  for(int i=0;i<c;i++){
    pthread_create(&consumer_threads[i],NULL,consumer,NULL);
  }  
  sleep(sleepTime);
  flag=true;
  for(int i=0;i<p;i++){
    pthread_join(producer_threads[i],NULL);
  }
  for(int i=0;i<c;i++){
    pthread_join(consumer_threads[i],NULL);
  }
  float throughput=(float)timeOfWork/(float)total_consumer_time.count();
  cout<<"Parameters used N = "<<N<<" p = "<<p<<" c = "<<c<<"\n";
  cout<<"Throughput of system = "<<throughput<<"\n";
  // exit(0);
  return 0;
}
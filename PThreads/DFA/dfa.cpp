#include <stdio.h>
#include <pthread.h>
#include<bits/stdc++.h>
#include <chrono> 
using namespace std;
using namespace std::chrono;
struct data{
  char *input;
  int l;
  int h;
  int noOfStates;
  int* initialStates;
  int* finalStates;
  int threads;
};
int getNextState(int currState, char symbol){
  if(currState==0){
    if(symbol=='0')return 1;
    if(symbol=='1')return 3;
  }
  else if(currState==2){
    if(symbol=='0')return 3;
    if(symbol=='1')return 1;
  }
  else if(currState==1){
    if(symbol=='0')return 0;
    if(symbol=='1')return 2;
  }
  else if(currState==3){
    if(symbol=='0')return 2;
    if(symbol=='1')return 0;
  }
}
void getFinalState(int* initialStates, int noOfStates, int *finalStates, char * input, int l, int h){
  for(int i=0;i<noOfStates;i++){
    int currState=initialStates[i];
    for(int j=l;j<h;j++){
      currState=getNextState(currState,input[j]);
    }
    finalStates[i]=currState;
  }
  // for(int i=0;i<noOfStates;i++)cout<<finalStates[i]<<"\n";
}
void *serialRunner(void *param){
  struct data *d=(struct data *)param;
  getFinalState(d->initialStates, d->noOfStates, d->finalStates, d->input, d->l, d->h);
  pthread_exit(0);
}
void *parallelRunner(void *param){
  struct data *d=(struct data *)param;
  pthread_t threads[d->threads];
  struct data ds[d->threads];
  int size=(d->h-d->l)/d->threads;
  int initialStates[d->threads][4];
  int finalStates[d->threads][4];
  for(int i=0;i<d->threads;i++){
    for(int j=0;j<4;j++){
      initialStates[i][j]=j;
      finalStates[i][j]=-1;
    }
  }
  for(int i=0;i<d->threads;i++){
    int l=i*size,h=(i+1)*size;
    // cout<<finalStates<<"\n";
    if(i==0){
      ds[i]={d->input,l,h,1,initialStates[i],finalStates[i],-1};  
    }
    else{
      ds[i]={d->input,l,h,4,initialStates[i],finalStates[i],-1};
    }
    pthread_create(&threads[i],NULL,serialRunner,(void*)&ds[i]);    
  }
  for(int i=0;i<d->threads;i++){
    if(i==0)
    pthread_join(threads[i],NULL);
  }
  int ans=d->initialStates[0];
  for(int i=0;i<d->threads;i++){
    ans=ds[i].finalStates[ans];
  }
  d->finalStates[0]=ans;
}
void generateBinaryString(char* input, int n){
  for(int i=0;i<n;i++){
    double x=rand()/double(RAND_MAX);
    if(x>0.5)input[i]='1';
    else input[i]='0';
  }
  input[0]='1';input[1]='0';input[2]='1';input[3]='1';
}
void printArray(char* input, int l,int h){
  for(int i=l;i<h;i++)cout<<input[i]<<" ";
  cout<<"\n";
}
int main(int argc ,char** argv){
  freopen("output2.txt","a",stdout);
  srand(time(0));
  int initialStates[1];initialStates[0]=0;
  int n=stoi(argv[1]);
  char input[n];
  int threads=stoi(argv[2]);
  printf("parameters used word size = %d and no of threads = %d\n", n, threads);
  generateBinaryString(input, n);
  // printArray(input,0,n);
  int finalStates1[1],finalStates2[1];
  pthread_t thread1,thread2;
  struct data d1,d2;
  
  d1={input,0,n,1,initialStates,finalStates1,-1};
  d2={input,0,n,1,initialStates,finalStates2,threads};

  auto start = high_resolution_clock::now(); 
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<nanoseconds>(stop - start); 
  start = high_resolution_clock::now();
  pthread_create(&thread1,NULL,serialRunner,(void*)&d1);
  pthread_join(thread1,NULL);
  stop = high_resolution_clock::now(); 
  duration = duration_cast<nanoseconds>(stop - start); 
  cout << "Time taken by sequencial implementation : "<< duration.count() << " nanoseconds" << endl; 
  // cout<<"ans "<<d1.finalStates[0]<<"\n";

  start = high_resolution_clock::now();
  pthread_create(&thread2,NULL,parallelRunner,(void*)&d2);
  pthread_join(thread2,NULL);
  stop = high_resolution_clock::now(); 
  duration = duration_cast<nanoseconds>(stop - start); 
  cout << "Time taken by parallel implementation : "<< duration.count() << " nanoseconds" << endl; 
  return 0;
}
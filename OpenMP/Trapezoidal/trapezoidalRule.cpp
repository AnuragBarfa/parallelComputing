#include <stdio.h>
#include<omp.h>
#include<bits/stdc++.h>
#include <chrono> 
using namespace std;
using namespace std::chrono;
double func(double x){
	return 4/(1+x*x);
}
double funcIntegration(double x){
	return x*x*x/3;
}
double actualAnswer(double a, double b){
	return 3.14159265358979323846 ;
}
double routine(double x, double dx, int n){
	double local_ans=0;
	for(int i=0;i<n;i++){
		local_ans+=func(x);
		x+=dx;
	}
	return local_ans*dx;
}
void parallelTrap(double a,double b,int n, double *final_ans){
	// printf("%s\n", "sd");
	int my_rank = omp_get_thread_num();
	int thread_count = omp_get_num_threads();
	// printf("%d %d\n", my_rank, thread_count);
	int new_n=n/thread_count;
	double dx=(b-a)/n;
	double local_ans=routine(a+my_rank*new_n*dx, dx, new_n);
	#pragma omp critical
	*final_ans += local_ans;
}
double simpleTrap(double a,double b,int n){
	double ans=routine(a, (b-a)/n, n);
	return ans;
}
int main(int argc ,char** argv){
	freopen("output2.txt","a",stdout);
	cout << std::setprecision(20);
	double a,b;
	int n;// n must be divisible by thread_count
	int thread_count;
	a=0;b=1;//n=400;thread_count=8;
	// scanf("%d",&n);
	// scanf("%d",&thread_count);
	n=stoi(argv[1]);
	thread_count=stoi(argv[2]);
	printf("parameters used n = %d\n", n);
	printf("parameters used thread_count = %d\n", thread_count);
	double final_ans=0;
	auto start = high_resolution_clock::now(); 
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds>(stop - start); 
	start = high_resolution_clock::now();
	#pragma omp parallel num_threads(thread_count)
	parallelTrap(a,b,n,&final_ans);
	stop = high_resolution_clock::now(); 
	duration = duration_cast<nanoseconds>(stop - start); 
  	cout << "Time taken by parallel implementation : "<< duration.count() << " nanoseconds" << endl; 
  	
  	start = high_resolution_clock::now();
	double final_ans2=simpleTrap(a,b,n);
	stop = high_resolution_clock::now(); 
	duration = duration_cast<nanoseconds>(stop - start); 
  	cout << "Time taken by serial implementation : "<< duration.count() << " nanoseconds" << endl; 
  	
  	// cout<<"Actual Answer : "<< actualAnswer(a,b)<<endl;
  	// cout<<"Answer found by parallel implementation : "<<final_ans<<endl;
  	// cout<<"Answer found by simple implementation : "<<final_ans2<<endl;
	return 0;
}
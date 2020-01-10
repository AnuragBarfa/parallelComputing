#include <bits/stdc++.h>
#include <chrono> 
using namespace std;
using namespace std::chrono;
void initialize(int n,vector<vector<double> > &A, vector<vector<double> > &B){
	for(int i=0;i<n;i++){
		//vector<double> temp(n);
		for(int j=0;j<n;j++){
			A[i][j]=rand()%50;
			B[i][j]=rand()%50;
		}
	}
}
vector<vector<double> > naiveMulti(int n,vector<vector<double> > A, vector<vector<double> > B){
	vector<vector<double> > C(n,vector<double> (n,0));
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			for(int k=0;k<n;k++){
				C[i][j]+=A[i][k]*B[k][j];
			}
		}
	}
	return C;
}
vector<vector<double> > opti1Multi(int n,vector<vector<double> > A, vector<vector<double> > B){
	vector<vector<double> > C(n,vector<double> (n,0));
	for(int i=0;i<n;i++){
		for(int k=0;k<n;k++){
			for(int j=0;j<n;j++){
				C[i][j]+=A[i][k]*B[k][j];
			}
		}
	}
	return C;
}
int main(){
	for(int n=2;n<4;n+=2){
		vector<vector<double> > A(n,vector<double> (n,0));
		vector<vector<double> > B(n,vector<double> (n,0));
		initialize(n,A,B);
		auto start = high_resolution_clock::now(); 
		vector<vector<double> >C1 = naiveMulti(n,A,B);
		auto stop = high_resolution_clock::now(); 
		auto duration = duration_cast<nanoseconds>(stop - start); 
  		cout << "Time taken by function: "<< duration.count() << " nanoseconds" << endl; 
    	for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				cout<<C1[i][j]<<" ";
			}
			cout<<"\n";
		}
    	start = high_resolution_clock::now(); 
		vector<vector<double> >C2 = opti1Multi(n,A,B);
		stop = high_resolution_clock::now(); 
		duration = duration_cast<nanoseconds>(stop - start); 
    	cout << "Time taken by function: "<< duration.count() << " nanoseconds" << endl;
    	for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				cout<<C2[i][j]<<" ";
			}
			cout<<"\n";
		}
		start = high_resolution_clock::now(); 
		vector<vector<double> >C3 = opti1Multi(n,A,B);
		stop = high_resolution_clock::now(); 
		duration = duration_cast<nanoseconds>(stop - start); 
    	cout << "Time taken by function: "<< duration.count() << " nanoseconds" << endl;
    	for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				cout<<C3[i][j]<<" ";
			}
			cout<<"\n";
		}
	}
	return 0;
}

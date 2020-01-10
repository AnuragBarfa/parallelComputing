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
vector<vector<double> > opti2Multi(int n, int s, vector<vector<double> > A, vector<vector<double> > B){
	vector<vector<double> > C(n,vector<double> (n,0));
	for(int i=0;i<n/s;i++){
		for(int j=0;j<n/s;j++){
			for(int k=0;k<n/s;k++){
				for(int i1=(i)*s;i1<(i+1)*s;i1++){
					for(int j1=(j)*s;j1<(j+1)*s;j1++){
						for(int k1=(k)*s;k1<(k+1)*s;k1++){
							C[i1][j1]+=A[i1][k1]*B[k1][j1];
						}
					}
				}
			}
		}
	}
	return C;
}
vector<vector<double> > opti3Multi(int n,vector<vector<double> > A, int B_l, int A_r, int A_h, int A_r, vector<vector<double> > B, int B_l, int B_r, int B_h, int B_r){
	int A_mid_x=(A_l+A_r+1)/2;
	int A_mid_y=(A_h+A_l+1)/2;
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
void display(vector<vector<double> > C,int n){
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			cout<<C[i][j]<<" ";
		}
		cout<<"\n";
	}
}
int main(){
	for(int n=2;n<=1024;n*=2){
		cout<<"for size "<<n<<"\n";
		vector<vector<double> > A(n,vector<double> (n,0));
		vector<vector<double> > B(n,vector<double> (n,0));
		initialize(n,A,B);
		auto start = high_resolution_clock::now(); 
		vector<vector<double> >C1 = naiveMulti(n,A,B);
		auto stop = high_resolution_clock::now(); 
		auto duration = duration_cast<nanoseconds>(stop - start); 
  		cout << "Time taken by function: "<< duration.count() << " nanoseconds" << endl; 
  		// display(C1,n);
    	start = high_resolution_clock::now(); 
		vector<vector<double> >C2 = opti1Multi(n,A,B);
		stop = high_resolution_clock::now(); 
		duration = duration_cast<nanoseconds>(stop - start); 
    	cout << "Time taken by function: "<< duration.count() << " nanoseconds" << endl;
 		// display(C2,n);
		start = high_resolution_clock::now(); 
		int s=64;//s is the size of block of matrix we are multiplying
		vector<vector<double> >C3 = opti2Multi(n,min(s,n),A,B);
		stop = high_resolution_clock::now(); 
		duration = duration_cast<nanoseconds>(stop - start); 
    	cout << "Time taken by function: "<< duration.count() << " nanoseconds" << endl;
    	// display(C3,n);
    	start = high_resolution_clock::now(); 
		int s=64;//s is the size of block of matrix we are multiplying
		vector<vector<double> >C4 = opti3Multi(n,A,0,n-1,0,n-1,B,0,n-1,0,n-1);
		stop = high_resolution_clock::now(); 
		duration = duration_cast<nanoseconds>(stop - start); 
    	cout << "Time taken by function: "<< duration.count() << " nanoseconds" << endl;
    	display(C4,n);

	}
	return 0;
}

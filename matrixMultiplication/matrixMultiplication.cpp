#include <bits/stdc++.h>
#include <chrono> 
using namespace std;
using namespace std::chrono;
void initialize(int n,vector<vector<double> > &A, vector<vector<double> > &B){
	for(int i=0;i<n;i++){
		//vector<double> temp(n);
		for(int j=0;j<n;j++){
			A[i][j]=rand()%10;
			B[i][j]=rand()%10;
		}
	}
}
void naiveMulti(int n,vector<vector<double> > &A, vector<vector<double> > &B, vector<vector<double> > &C){
	// vector<vector<double> > C(n,vector<double> (n,0));
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			for(int k=0;k<n;k++){
				C[i][j]+=A[i][k]*B[k][j];
			}
		}
	}
}
void opti1Multi(int n,vector<vector<double> > &A, vector<vector<double> > &B, vector<vector<double> > &C){
	// vector<vector<double> > C(n,vector<double> (n,0));
	for(int i=0;i<n;i++){
		for(int k=0;k<n;k++){
			for(int j=0;j<n;j++){
				C[i][j]+=A[i][k]*B[k][j];
			}
		}
	}
}
void opti2Multi(int n, int s, vector<vector<double> > &A, vector<vector<double> > &B, vector<vector<double> > &C){
	// vector<vector<double> > C(n,vector<double> (n,0));
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
}
void opti3Multi(int n,vector<vector<double> > &A, int A_l, int A_r, int A_u, int A_d, vector<vector<double> > &B, int B_l, int B_r, int B_u, int B_d, vector<vector<double> > &C, int C_l, int C_r, int C_u, int C_d){
	int A_mid_x=(A_l+A_r)/2;
	int A_mid_y=(A_u+A_d)/2;
	int B_mid_x=(B_l+B_r)/2;
	int B_mid_y=(B_u+B_d)/2;
	int C_mid_x=(C_l+C_r)/2;
	int C_mid_y=(C_u+C_d)/2;
	// cout<<"in rec\n";
	// cout<<A_l<<" "<<A_r<<" "<<A_u<<" "<<A_d<<"\n";
	// cout<<B_l<<" "<<B_r<<" "<<B_u<<" "<<B_d<<"\n";
	// cout<<C_l<<" "<<C_r<<" "<<C_u<<" "<<C_d<<"\n";
	if(A_l==A_r){
		//base case
		C[C_u][C_l]+=A[A_u][A_l]*B[B_u][B_l];
		// cout<<C_l<<" "<<C_u<<" "<<A[A_l][A_u]<<" "<<B[B_l][B_u]<<"\n";
		return; 
	}
	opti3Multi(n, A, A_l, A_mid_x, A_u, A_mid_y, B, B_l, B_mid_x, B_u, B_mid_y, C, C_l, C_mid_x, C_u, C_mid_y);
	opti3Multi(n, A, A_mid_x+1, A_r, A_u, A_mid_y, B, B_l, B_mid_x, B_mid_y+1, B_d, C, C_l, C_mid_x, C_u, C_mid_y);
	opti3Multi(n, A, A_l, A_mid_x, A_u, A_mid_y, B, B_mid_x+1, B_r, B_u, B_mid_y, C, C_mid_x+1, C_r, C_u, C_mid_y);
	opti3Multi(n, A, A_mid_x+1, A_r, A_u, A_mid_y, B, B_mid_x+1, B_r, B_mid_y+1, B_d, C, C_mid_x+1, C_r, C_u, C_mid_y);

	opti3Multi(n, A, A_l, A_mid_x, A_mid_y+1, A_d, B, B_l, B_mid_x, B_u, B_mid_y, C, C_l, C_mid_x, C_mid_y+1, C_d);
	opti3Multi(n, A, A_mid_x+1, A_r, A_mid_y+1, A_d, B, B_l, B_mid_x, B_mid_y+1, B_d, C, C_l, C_mid_x, C_mid_y+1, C_d);
	opti3Multi(n, A, A_l, A_mid_x, A_mid_y+1, A_d, B, B_mid_x+1, B_r, B_u, B_mid_y, C, C_mid_x+1, C_r, C_mid_y+1, C_d);
	opti3Multi(n, A, A_mid_x+1, A_r, A_mid_y+1, A_d, B, B_mid_x+1, B_r, B_mid_y+1, B_d, C, C_mid_x+1, C_r, C_mid_y+1, C_d);
		
}
void display(vector<vector<double> > C,int n){
	cout<<"priting matrix\n";
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			cout<<C[i][j]<<" ";
		}
		cout<<"\n";
	}
}
int main(){
	for(int n=4096;n<=4096;n*=2){
		cout<<"for size "<<n<<"\n";
		vector<vector<double> > A(n,vector<double> (n,0));
		vector<vector<double> > B(n,vector<double> (n,0));
		initialize(n,A,B);
		// display(A,n);
		// display(B,n);
		auto start = high_resolution_clock::now(); 
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<nanoseconds>(stop - start); 
		
		start = high_resolution_clock::now();
		vector<vector<double> >C1(n,vector<double> (n,0));
		naiveMulti(n,A,B,C1); 
		stop = high_resolution_clock::now(); 
		duration = duration_cast<nanoseconds>(stop - start); 
  		cout << "Time taken by function: "<< duration.count() << " nanoseconds" << endl; 
  		// display(C1,n);

    	start = high_resolution_clock::now(); 
		vector<vector<double> >C2(n,vector<double> (n,0)); 
		opti1Multi(n,A,B,C2);
		stop = high_resolution_clock::now(); 
		duration = duration_cast<nanoseconds>(stop - start); 
    	cout << "Time taken by function: "<< duration.count() << " nanoseconds" << endl;
 		// display(C2,n);

		start = high_resolution_clock::now(); 
		int s=64;//s is the size of block of matrix we are multiplying
		vector<vector<double> >C3(n,vector<double> (n,0));
		opti2Multi(n,min(s,n),A,B,C3);
		stop = high_resolution_clock::now(); 
		duration = duration_cast<nanoseconds>(stop - start); 
    	cout << "Time taken by function: "<< duration.count() << " nanoseconds" << endl;
    	// display(C3,n);
    	start = high_resolution_clock::now(); 
		vector<vector<double> >C4(n,vector<double> (n,0));
		opti3Multi(n,A,0,n-1,0,n-1,B,0,n-1,0,n-1,C4,0,n-1,0,n-1);
		stop = high_resolution_clock::now(); 
		duration = duration_cast<nanoseconds>(stop - start); 
    	cout << "Time taken by function: "<< duration.count() << " nanoseconds" << endl;
    	// display(C4,n);

	}
	return 0;
}

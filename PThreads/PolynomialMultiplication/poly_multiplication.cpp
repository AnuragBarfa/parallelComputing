#include <stdio.h>
#include <pthread.h>
#include<bits/stdc++.h>
#include <chrono> 
using namespace std;
using namespace std::chrono;
void *runner(void *param);
void printArray(int *arr,int l,int h){
	for(int i=l;i<h;i++)printf("%d ", arr[i]);
	printf("\n");
}
struct data
{
	int *poly1;
	int l1;
	int h1;
	int* poly2;
	int l2;
	int h2;
	int* poly3;
};
struct naiveData
{
	int *poly1;
	int l1;
	int h1;
	int* poly2;
	int l2;
	int h2;
	int* poly3;
	int i;
};
void *runnerNaive(void *param){
	struct naiveData *d=(struct naiveData *)param;
	int n1=(d->h1)-(d->l1);
	int n2=(d->h2)-(d->l2);
	int i=d->i;
	int temp=0;
	for(int i1=0;(i1<=i and i1<n1);i1++){
		int i2=i-i1;
		if(i2<n2)temp+=(d->poly1)[i1+d->l1]*(d->poly2)[i2+d->l2];
		// printf("%d, %d, %d, %d\n",i1, i2, poly1[i1], poly2[i2]);
	}
	d->poly3[i]=temp;
	pthread_exit(0);
}
int* naive(int *poly1, int l1, int h1, int* poly2, int l2, int h2){
	int n1=h1-l1;
	int n2=h2-l2;
	int n3=n1+n2-1;
	int* ans= new int[n3];
	pthread_t threads[n3];
	struct naiveData datas[n3];
	for(int i=0;i<n3;i++){
		datas[i].poly1=poly1;datas[i].l1=l1;datas[i].h1=h1;
		datas[i].poly2=poly2;datas[i].l2=l2;datas[i].h2=h2;
		datas[i].poly3=ans;datas[i].i=i;
		pthread_create(threads+i,NULL,runnerNaive,(void*)(datas+i));
	}
	for(int i=0;i<n3;i++){
		pthread_join(threads[i] ,NULL);
	}
	return ans;
}
int *addPoly(int *poly1, int l1, int h1, int* poly2, int l2, int h2){
	int len=max(h2-l2,h1-l1);
	// printf("in add %d\n", len);
	int *ans=new int[len];
	for(int i=0;i<len;i++)ans[i]=0;
	for(int i=l1;i<h1;i++)ans[i-l1]+=poly1[i];
	for(int i=l2;i<h2;i++)ans[i-l2]+=poly2[i];
	return ans;
}
int *subPoly(int *poly1, int l1, int h1, int* poly2, int l2, int h2){
	int len=max(h2-l2,h1-l1);
	int *ans=new int[len];
	// printf("in sub %d\n", len);
	for(int i=0;i<len;i++)ans[i]=0;
	for(int i=l1;i<h1;i++)ans[i-l1]+=poly1[i];
	for(int i=l2;i<h2;i++)ans[i-l2]-=poly2[i];
	return ans;
} 
int *shiftPoly(int *poly,int n,int shift){
	int *ans=new int[shift+n];
	for(int i=0;i<shift;i++)ans[i]=0;
	for(int i=shift;i<shift+n;i++)ans[i]=poly[i-shift];
	return ans;
}
int* Karatsuba(int *poly1, int l1, int h1, int* poly2, int l2, int h2){
	// printf("int karastuba %d %d\n",h1-l1,h2-l2);
	// printArray(poly1,l1,h1);
	// printArray(poly2,l2,h2);
	int *ans;
	if(h1-l1<100||h2-l2<100){
		ans=naive(poly1, l1, h1, poly2, l2, h2);
	}
	else{
		int m1=(l1+h1)/2;
		if(m1-l1>h2-l2){
			// case we want to consume complete or more of poly2
			m1=l1+(l2-h2);//leave one element in right parition for poly2 above if else make sure length is more than 1
		}
		int shift=m1-l1;
		int m2=m1;

		pthread_t thread1;
		struct data d1;
		d1.poly1=poly1;
		d1.l1=l1;d1.h1=m1;
		d1.l2=l2;d1.h2=m2;
		d1.poly2=poly2;
		pthread_create(&thread1,NULL,runner,(void*)&d1);
		// printArray(I1_J1,0,i1);
		
		pthread_t thread2;
		struct data d2;
		d2.poly1=poly1;
		d2.l1=m1;d2.h1=h1;
		d2.l2=m2;d2.h2=h2;
		d2.poly2=poly2;
		pthread_create(&thread2,NULL,runner,(void*)&d2);
		
		// printArray(I2_J2,0,i2);
		int *temp1=addPoly(poly1,l1,m1,poly1,m1,h1);
		int t1=max(h1-m1,m1-l1);
		int *temp2=addPoly(poly2,l2,m2,poly2,m2,h2);
		int t2=max(h2-m2,m2-l2);
		
		pthread_t thread3;
		struct data d3;
		d3.poly1=temp1;
		d3.l1=0;d3.h1=t1;
		d3.l2=0;d3.h2=t2;
		d3.poly2=temp2;
		pthread_create(&thread3,NULL,runner,(void*)&d3);
		pthread_join(thread1 ,NULL);
		pthread_join(thread2 ,NULL);
		pthread_join(thread3 ,NULL);
		delete[] temp1,temp2;
		int *I1_J1=d1.poly3;//Karatsuba(poly1,l1,m1,poly2,l2,m2);
		int i1=m1-l1+m2-l2-1;
		int *I2_J2=d2.poly3;//Karatsuba(poly1,m1,h1,poly2,m2,h2);
		int i2=h1-m1+h2-m2-1;
		int *I1_2_J1_2=d3.poly3;//Karatsuba(temp1,0,t1,temp2,0,t2);
		int i3=t1+t2-1;
		
		// printf("i3\n");
		// printArray(I1_2_J1_2,0,i3);
		int *temp3=addPoly(I1_J1,0,i1,I2_J2,0,i2);
		int t3=max(i1,i2);
		// printf("t3\n");
		// printArray(temp3,0,t3);
		int *I12_J21=subPoly(I1_2_J1_2,0,i3,temp3,0,t3);
		int i4=max(i3,t3);
		// printf("i4\n");
		// printArray(I12_J21,0,i4);
		int *Shifted_I12_J21=shiftPoly(I12_J21,i4,shift);
		int s1=i4+shift;
		// printf("s1\n");
		// printArray(Shifted_I12_J21,0,s1); 
		int *Shifted_I2_J2=shiftPoly(I2_J2,i2,2*shift);
		int s2=i2+2*shift;
		// printf("s2\n");
		// printArray(Shifted_I2_J2,0,s2);
		ans=addPoly(Shifted_I12_J21,0,s1,Shifted_I2_J2,0,s2);
		int a1=max(s1,s2);
		ans=addPoly(ans,0,a1,I1_J1,0,i1);
		delete Shifted_I2_J2,Shifted_I12_J21,I12_J21,temp3,I1_2_J1_2,I2_J2,I1_J1;
		// printf("%s\n", "dfsdf");
	}
	return ans;
}
void *runner(void *param){
	struct data *d=(struct data *)param;
	d->poly3=Karatsuba(d->poly1,d->l1,d->h1,d->poly2,d->l2,d->h2);
	pthread_exit(0);
}
void randomArray(int* arr,int l,int h){
	for(int i=l;i<h;i++){
		arr[i]=rand()%10;
	}
}
int main(int argc ,char** argv){
	freopen("output2.txt","a",stdout);
	cout << std::setprecision(20);
	int n1,n2;
	n1=n2=stoi(argv[1]);
	// n1=2;
	// n2=4;
	// scanf("%d",&n1);
	// scanf("%d",&n2);
	int *poly1=new int[n1];
	int *poly2=new int[n2];

	poly1[0]=1;poly1[1]=2;poly1[2]=3;poly1[3]=4;
	poly2[0]=2;poly2[1]=1;poly2[2]=3;poly2[3]=1;
	// randomArray(poly1,0,n1);
	// randomArray(poly2,0,n2);
	printf("parameters used polynomial size = %d\n", n1);
	// printArray(poly1,0,n1);
	// printArray(poly2,0,n2);
	auto start = high_resolution_clock::now(); 
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds>(stop - start); 
	start = high_resolution_clock::now();
	int *ans2=naive(poly1,0,n1,poly2,0,n2);
	stop = high_resolution_clock::now(); 
	duration = duration_cast<nanoseconds>(stop - start); 
  	cout << "Time taken by naive implementation : "<< duration.count() << " nanoseconds" << endl; 
	// printArray(ans2,0,n1+n2-1);

	pthread_t thread;
	struct data d;
	d.poly1=poly1;
	d.l1=0;d.h1=n1;
	d.l2=0;d.h2=n2;
	d.poly2=poly2;
	start = high_resolution_clock::now();
	pthread_create(&thread,NULL,runner,(void*)&d);
	pthread_join(thread,NULL);
	stop = high_resolution_clock::now(); 
	duration = duration_cast<nanoseconds>(stop - start); 
  	cout << "Time taken by parallel implementation : "<< duration.count() << " nanoseconds" << endl; 
	// printArray(d.poly3,0,n1+n2-1);
	return 0;
}
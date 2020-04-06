#include <stdio.h>
#include<omp.h>
#include<bits/stdc++.h>
using namespace std;
void printArray(int *arr,int l,int h){
	for(int i=l;i<h;i++)printf("%d ", arr[i]);
	printf("\n");
}
int* simple(int *poly1, int n1, int* poly2, int n2){
	int n3=n1+n2-1;
	int* ans= new int[n3];
	for(int i=0;i<n3;i++){
		int temp=0;
		// printf("for power : %d\n", i);
		for(int i1=0;(i1<=i and i1<n1);i1++){
			int i2=i-i1;
			if(i2<n2)temp+=poly1[i1]*poly2[i2];
			// printf("%d, %d, %d, %d\n",i1, i2, poly1[i1], poly2[i2]);
		}
		ans[i]=temp;
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
	if(l1==h1|| l2==h2){
		ans=new int[1];
		ans[0]=0;
	}
	else if(h1-l1==1){
		ans=new int[h2-l2];
		for(int i=l2;i<h2;i++)ans[i-l2]=poly2[i]*poly1[l1];
	}
	else if(h2-l2==1){
		ans=new int[h1-l1];
		for(int i=l1;i<h1;i++)ans[i-l1]=poly1[i]*poly2[l2];
	}
	else{
		int m1=(l1+h1)/2;
		if(m1-l1>h2-l2){
			// case we want to consume complete or more of poly2
			m1=l1+(l2-h2);//leave one element in right parition for poly2 above if else make sure length is more than 1
		}
		int shift=m1-l1;
		int m2=m1;
		int *I1_J1=Karatsuba(poly1,l1,m1,poly2,l2,m2);
		int i1=m1-l1+m2-l2-1;
		// printArray(I1_J1,0,i1);
		int *I2_J2=Karatsuba(poly1,m1,h1,poly2,m2,h2);
		int i2=h1-m1+h2-m2-1;
		// printArray(I2_J2,0,i2);
		int *temp1=addPoly(poly1,l1,m1,poly1,m1,h1);
		int t1=max(h1-m1,m1-l1);
		int *temp2=addPoly(poly2,l2,m2,poly2,m2,h2);
		int t2=max(h2-m2,m2-l2);
		int *I1_2_J1_2=Karatsuba(temp1,0,t1,temp2,0,t2);
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
		// printf("%s\n","beforeans" );
		ans=addPoly(Shifted_I12_J21,0,s1,Shifted_I2_J2,0,s2);
		// printf("%s\n","after" );
		int a1=max(s1,s2);
		ans=addPoly(ans,0,a1,I1_J1,0,i1);
		// printf("%s\n", "dfsdf");
	}
	return ans;
}

void randomArray(int* arr,int l,int h){
	for(int i=l;i<h;i++){
		arr[i]=rand()%10;
	}
}
int main(){
	int n1,n2;
	// n1=2;
	// n2=4;
	scanf("%d",&n1);
	scanf("%d",&n2);
	int *poly1=new int[n1];
	int *poly2=new int[n2];
	randomArray(poly1,0,n1);
	randomArray(poly2,0,n2);
	printArray(poly1,0,n1);
	printArray(poly2,0,n2);
	int *ans2=simple(poly1,n1,poly2,n2);
	int *ans=Karatsuba(poly1,0,n1,poly2,0,n2);
	printArray(ans,0,n1+n2-1);
	printArray(ans2,0,n1+n2-1);
	return 0;
}
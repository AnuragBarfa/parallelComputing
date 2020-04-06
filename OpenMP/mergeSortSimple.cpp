#include <stdio.h>
#include<omp.h>
#include<bits/stdc++.h>
void printArray(int *arr ,int l, int h){
	for(int i=l;i<=h;i++)printf("%d ", arr[i]);
	printf("\n");
}
int lastFound(int *arr, int l, int h, int x){
	if(l==h){
		if(arr[l]==x)return l;
		else return -1;
	}
	if(l<h){
		int mid=(l+h)/2;
		if(arr[mid]<=x){
			return lastFound(arr,mid,h,x);
		}
		else{
			return lastFound(arr,l,mid-1,x);
		}
	}
}
void merge(int *arr, int l1, int h1, int l2, int h2){
	int *temp = new int[h1+h2-l1-l2+2];
	int i1=l1,i2=l2;
	int index=0;
	while(i1<=h1||i2<=h2){
		if(i1==h1+1){
			temp[index]=arr[i2];
			i2=i2+1;
		}
		else if(i2==h2+1){
			temp[index]=arr[i1];
			i1=i1+1;
		}
		else{
			if(arr[i1]<=arr[i2]){
				temp[index]=arr[i1];
				i1=i1+1;
			}
			else{
				temp[index]=arr[i2];
				i2=i2+1;
			}
		}
		index++;
	}
	for(int i=l1;i<=h2;i++)arr[i]=temp[i-l1];
	return ;
}
void parallelMerge(int *arr, int l1, int h1, int l2, int h2, int start){
	if(h1<l1||h2<l2)return;
	int m1=(h1+l1)/2;
	int x=arr[m1];
	int m2=lastFound(arr,l2,h2,x);
	int x_pos=start+(m1-1-l1+1)+(m2-l2+1);
	arr[x_pos]=x;
	printf("%d %d\n", m1, m2);
	parallelMerge(arr,l1,m1-1,l2,m2,start);
	parallelMerge(arr,m1+1,h1,m2+1,h2,x_pos+1);
}
void mergeSort(int* arr,int l ,int h, int lev){
	if(l==h)return ;
	if(l<h){
		int mid=(l+h)/2;
		mergeSort(arr, l, mid, lev+1);
		mergeSort(arr, mid+1, h, lev+1);
		merge(arr, l, mid, mid+1, h);
		return ;
	}
}
int main(){
	int n=8;
	int *arr = new int[n];
	for(int i=0;i<n;i++){
		// arr[i]=rand()%100;
		arr[i]=i%(n/2);
	}
	printArray(arr,0,n-1);
	// mergeSort(arr, 0, n-1, 0);
	parallelMerge(arr,0,n/2-1,n/2,n-1,0);
	printArray(arr,0,n-1);
	return 0;
}
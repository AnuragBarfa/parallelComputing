#include <stdio.h>
#include <pthread.h>
#include<bits/stdc++.h>
#include <chrono> 
using namespace std;
using namespace std::chrono;
void *runner(void *param);
class complexData{
	public:
		double x,y;
		complexData(){
			x=0;y=0;
		}
		complexData(double a,double b){
			x=a;y=b;
		}
		complexData add(complexData d){
			return complexData(x+d.x,y+d.y);
		}
		complexData sub(complexData d){
			return complexData(x-d.x,y-d.y);
		}
		complexData mul(complexData d){
			return complexData(x*d.x-y*d.y,x*d.y+d.x*y);
		}
		void set(double a,double b){
			x=a;y=b;
		}
		void set(complexData d){
			x=d.x;y=d.y;
		}

};
struct data
{
	complexData *poly;
	int size;
	complexData w;
};
void printComplexData(complexData d){
	cout<<"{ "<<d.x<<","<<d.y<<" }";
}
void printArray(complexData *arr,int l,int h){
	for(int i=l;i<h;i++){
		printComplexData(arr[i]);
		cout<<" ";
	}
	printf("\n");
}
void recursive_FFT(complexData *poly,int n,complexData w){
	if(n==1)return ;
	// expecting n as power of 2
	complexData even_poly[n/2];
	complexData odd_poly[n/2];
	for(int i=0;i<n/2;i++){
		even_poly[i]=poly[2*i];
		odd_poly[i]=poly[2*i+1];
	}
	pthread_t threads[2];
	struct data d0,d1;
	d0.poly=even_poly;d0.size=n/2;d0.w=w.mul(w);
	d1.poly=odd_poly;d1.size=n/2;d1.w=w.mul(w);
	pthread_create(&threads[0],NULL,runner,(void*)&d0);// recursive_FFT(even_poly,n/2,w.mul(w));
	pthread_create(&threads[1],NULL,runner,(void*)&d1);// recursive_FFT(odd_poly,n/2,w.mul(w));
	complexData x(1,0);//largest entry of remaining x will be one and will decrease by a factor of w
	pthread_join(threads[0],NULL);
	pthread_join(threads[1],NULL);
	for(int i=0;i<n/2;i++){
		poly[i]=even_poly[i].add(x.mul(odd_poly[i]));
		poly[n/2+i]=even_poly[i].sub(x.mul(odd_poly[i]));
		x=x.mul(w);
	}
	return ;
}
void *runner(void *param){
	struct data *d=(struct data *)param;
	recursive_FFT(d->poly,d->size,d->w);
	pthread_exit(0);
}
int main(){
	int n1,n2;
	n1=n2=4;
	complexData poly1[n1];
	complexData poly2[n2];
	poly1[0].set(1,0);poly1[1].set(2,0);poly1[2].set(3,0);poly1[3].set(4,0);
	poly2[0].set(2,0);poly2[1].set(1,0);poly2[2].set(3,0);poly2[3].set(1,0);
	int n3=n1+n2;
	complexData updated_poly1[n3];
	complexData updated_poly2[n3];
	for(int i=0;i<n1;i++)updated_poly1[i]=poly1[i];
	for(int i=0;i<n2;i++)updated_poly2[i]=poly2[i];
	complexData w(cos(2*M_PI/n3),sin(2*M_PI/n3));
	pthread_t threads[3];
	struct data d0,d1,d2;
	d0.poly=updated_poly1;d0.size=n3;d0.w=w;
	d1.poly=updated_poly2;d1.size=n3;d1.w=w;
	// printArray(updated_poly1,0,n3);
	pthread_create(&threads[0],NULL,runner,(void*)&d0);// recursive_FFT(updated_poly1,n3,w);
	// printArray(updated_poly1,0,n3);
	// printArray(updated_poly2,0,n3);
	pthread_create(&threads[1],NULL,runner,(void*)&d1);// recursive_FFT(updated_poly2,n3,w);
	// printArray(updated_poly2,0,n3);
	pthread_join(threads[0],NULL);
	pthread_join(threads[1],NULL);
	complexData poly_mul_value[n3];
	for(int i=0;i<n3;i++)poly_mul_value[i].set(updated_poly1[i].mul(updated_poly2[i]));
	// printArray(poly_mul_value,0,n3);
	complexData w1(cos(2*M_PI/n3),sin(-2*M_PI/n3));
	d2.poly=poly_mul_value;d2.size=n3;d2.w=w1;
	pthread_create(&threads[2],NULL,runner,(void*)&d2);// recursive_FFT(poly_mul_value,n3,w1);
	pthread_join(threads[2],NULL);
	for(int i=0;i<n3;i++)poly_mul_value[i].set(poly_mul_value[i].x/(n3),poly_mul_value[i].y/(n3));
	printArray(poly_mul_value,0,n3);
	return 0;
}
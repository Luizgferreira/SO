#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>


void * routine(void *arg){
	int * k = arg; 
	printf("Argumento: %d\n", *k);
	*k = 20;
	pthread_exit(arg);
}

void * result(void *arg){
	double * sum = arg;
	int m = 0;
	int i;
	double x, y, result;
	for(i = 0; i<500000000; i++){
		x = (double)rand()/(double)(RAND_MAX/1);
		y = (double)rand()/(double)(RAND_MAX/1);
		result = pow(x, 2.0) + pow(y, 2.0);
		if(result<1){
			m = m + 1;
		}
	}
	*sum = (double)m/i;
	pthread_exit(arg);
}

int main(int argc, char *argv[]){
	srand(time(NULL));
	pthread_t thread_idA;
	pthread_t thread_idB;
	void * thread_res;
	double r1, r2;
	//long* x;
	double i;
	double * x = &i;
	double i2;
	double * x2 = &i2;
	r1 = pthread_create (&thread_idA, NULL, result, (void*)x);
	r2 = pthread_create (&thread_idB, NULL, result, (void*)x2);
	r1 = pthread_join (thread_idA, &thread_res);
	r2 = pthread_join (thread_idB, &thread_res);
	printf("Argumento: %lf\n", 4*(i+i2));

}
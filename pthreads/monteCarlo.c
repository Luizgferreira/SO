#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#define N_CORE 4

void * result(void *arg){
	double * sum = arg;
	int m = 0;
	int i;
	double x, y, result;
	for(i = 0; i<250000000; i++){
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
	pthread_t thread_id[N_CORE];
	void * thread_res;
	double thread[N_CORE];
	double thread_result[N_CORE];
	//criar threads
	for (int i = 0; i < N_CORE; i++){
		thread[i] = pthread_create(&thread_id[i], NULL, result, (void*)&thread_result[i]);
		printf("Thread %d criada \n", i);
	}
	//finalizar threads
	for (int i = 0; i < N_CORE; i++){
		thread[i] = pthread_join(thread_id[i], &thread_res);
		printf("Thread %d finalizada \n", i);
	}
	printf("\n %lf", thread_result[0]+thread_result[1]+thread_result[2]+thread_result[3]);
	return 0;

}
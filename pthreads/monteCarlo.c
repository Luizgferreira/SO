#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#define N_CORE 4
#define N_ITER 1000000000


void * result(void *arg){
	double * sum = arg;
	int m = 0;
	int i;
	double x, y, result;
	for(i = 0; i<N_ITER/N_CORE; i++){
		x = (double)rand()/(double)(RAND_MAX/1);
		y = (double)rand()/(double)(RAND_MAX/1);
		result = pow(x, 2.0) + pow(y, 2.0);
		if(result<1){
			m = m + 1;
		}
	}
	*sum = (double)m/N_ITER;
	pthread_exit(arg);
}

int main(int argc, char *argv[]){
	if(argc!=2){
		printf("Número de entradas incorreto.\n");
		exit(1);
	}
	FILE *fp;
	fp = fopen(argv[1], "w");
	if(fp == NULL){
		printf("Erro de abertura do arquivo.\n");
		exit(1);
	}
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
	double result = 0;
	//finalizar threads
	for (int i = 0; i < N_CORE; i++){
		thread[i] = pthread_join(thread_id[i], &thread_res);
		printf("Thread %d finalizada \n", i);
		result = result + thread_result[i];

	}
	fprintf(fp, "%lf\n", 4*result);
	fclose(fp);
	return 0;
}
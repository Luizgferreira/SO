#include <pthread.h>
#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <math.h>

#define n_t 3
#define n_iter 100

void *threadA(void* arg){
	mpf_t *res = arg;
	mpf_t aux;
	mpf_init(aux);
	mpf_add(aux,res[0],res[1]);
	mpf_div_ui(res[2],aux,2);
	return NULL;
}

void *threadB(void* arg){
	mpf_t *res = arg;
	mpf_t aux;
	mpf_init(aux);
	mpf_mul(aux,res[0],res[1]);
	mpf_sqrt(res[3],aux);
	return NULL;
}
void *threadC(void* arg){
	mpf_t *res = arg;
	mpf_t aux;
	mpf_init(aux);
	mpf_mul_ui(res[6],res[4],2);
	return NULL;
}

void *threadD(void* arg){
	mpf_t *res = arg;
	mpf_t aux;
	mpf_init(aux);
	mpf_sub(aux,res[0],res[2]);
	mpf_pow_ui(aux,aux,2);

	//mpf_out_str (stdout, 10, 0, aux);printf("\n");


	mpf_mul(aux,aux,res[4]);
	
	//mpf_out_str (stdout, 10, 0, aux);printf("\n");

	mpf_sub(res[7],res[5],aux);
	
	return NULL;
}

int main(int argc, char *argv[]){
	if(argc!=3){
		printf("Número de entradas incorreto.\n");
		exit(1);
	}
	FILE *fp;
	fp = fopen(argv[2], "w");
	if(fp == NULL){
		printf("Erro de abertura do arquivo.\n");
		exit(1);
	}
	int i,j;
	//inicialização
	pthread_t threads[n_t];
	//a,b,ak,bk,p,t,pk,tk
	mpf_t *arg = malloc(8*sizeof(mpf_t));
	for(i=0;i<8;i++){
		mpf_init(arg[i]);
		//mpf_set_ui(arg[i],0);
		//printf("\n");
	}
	//colocar valores iniciais
	mpf_t aux;
	mpf_init(aux);
	mpf_set_ui(aux, 2);
	mpf_sqrt(aux,aux);
	mpf_ui_div(aux,1,aux);
	//mpf_out_str(stdout,10,0,aux);printf("\n");

	mpf_set_ui(arg[0],1);
	mpf_set(arg[1],aux);
	mpf_set_ui(arg[4],1);

	mpf_set_ui(aux,1);
	mpf_div_ui(arg[5],aux,4);

	//mpf_out_str(stdout,10,0,arg[5]);printf("\n");

	for(i=0; i<n_iter; i++){
		//tread1
		pthread_create(&threads[0], NULL, threadA, arg);
		//thread2
		pthread_create(&threads[1], NULL, threadB, arg);
		//thread3
		pthread_create(&threads[2], NULL, threadC, arg);


		//tirar thread1
		pthread_join(threads[0], NULL);
		

		//thread4
		pthread_create(&threads[3], NULL, threadD, arg);

		//tirar thread2
		pthread_join(threads[1], NULL);
		//tirar thread3
		pthread_join(threads[2], NULL);

		//tirar thread4
		pthread_join(threads[3], NULL);


		mpf_sub(aux,arg[0],arg[2]);

		//fazer um passo no tempo
		mpf_set(arg[0],arg[2]);
		mpf_set(arg[1],arg[3]);
		mpf_set(arg[4],arg[6]);
		mpf_set(arg[5],arg[7]);



		//mpf_out_str(stdout,10,0,aux);printf("\n");
		//mpf_out_str(stdout,10,0,arg[6]);printf("\n");
		

	}

	mpf_t pi;
	mpf_init(pi);
	mpf_add(aux,arg[2],arg[3]);
	mpf_pow_ui(aux,aux,2);
	mpf_div_ui(aux,aux,4);
	mpf_div(pi,aux,arg[7]);

	mpf_out_str(fp,10,0,pi);

	mpf_clear(aux);
	for(i=0;i<8;i++){
		mpf_clear(arg[i]);
	}
	return 0;
}

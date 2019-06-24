#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <gmp.h>
#include <pthread.h>

#define N_ITER 1000000


struct dados_threadA
{
	mpf_t a, y;
	int k;
};

struct dados_threadY
{
	mpf_t y, y_k2;
};

void * calcula_a(void *arg);
void * calcula_y(void *arg);

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

	//calcular primeiros valores de y
	mpf_t y, y_k1, y_k2, aux, aux2;
	//mpf_init(a);
	mpf_init(y);
	mpf_init(aux);
	mpf_init(aux2);
	mpf_init(y_k1);
	mpf_init(y_k2);

	mpf_set_d(y, sqrt(2)-1);
	//y1
	mpf_pow_ui(aux, y, 4);
	mpf_ui_sub(aux, 1.0, aux);
	mpf_sqrt(aux,aux);
	mpf_sqrt(aux,aux);
	mpf_ui_sub(aux2, 1.0, aux);
	mpf_add_ui(aux, aux, 1.0);
	mpf_div(y_k1, aux2, aux);
	//y2
	mpf_pow_ui(aux, y_k1, 4);
	mpf_ui_sub(aux, 1.0, aux);
	mpf_sqrt(aux,aux);
	mpf_sqrt(aux,aux);
	mpf_ui_sub(aux2, 1.0, aux);
	mpf_add_ui(aux, aux, 1.0);
	mpf_div(y_k2, aux2, aux);
	//aux e aux2 não serão mais necessárias
	mpf_clear(aux);
	mpf_clear(aux2);
	//insere y1 na struct que será passada a thread que calcula a

	struct dados_threadA data;
	mpf_init(data.a);
	mpf_init(data.y);
	mpf_set_d(data.a, 2*pow(sqrt(2)-1,2));
	
	
	//inicia operações nas threads
	pthread_t thread_id[2];
	void * thread_res;
	double thread[2];

	struct dados_threadY dados_y;

	mpf_init(dados_y.y);
	mpf_init(dados_y.y_k2);
	mpf_set(dados_y.y, y);
	mpf_set(dados_y.y_k2, y_k2);
	for (int i=0;i<N_ITER;i++){
		data.k = i;
		mpf_set(data.y, y_k1);

		thread[0] = pthread_create(&thread_id[0], NULL, calcula_a, (void*)&data);
		thread[1] = pthread_create(&thread_id[1], NULL, calcula_y, (void*)&dados_y);
		thread[0] = pthread_join(thread_id[0], &thread_res);
		thread[1] = pthread_join(thread_id[1], &thread_res);
		
		mpf_set(y_k1, y_k2);
		mpf_set(y_k2, dados_y.y);
	}
	mpf_t pi;
	mpf_init(pi);
	mpf_ui_div(pi, 1.0, data.a);
	mpf_out_str (fp, 10, 0, pi);
	mpf_clear(y);
	mpf_clear(y_k1);
	mpf_clear(y_k2);	
	mpf_clear(data.a);
	mpf_clear(data.y);
	mpf_clear(dados_y.y);
	mpf_clear(dados_y.y_k2);

	return 0;
}

void * calcula_a(void *arg){
	struct dados_threadA * dados = arg;
	mpf_t aux, aux2, aux3;
	mpf_init(aux);
	mpf_init(aux2);
	mpf_init(aux3);

	mpf_add_ui(aux,dados->y, 1.0);
	mpf_pow_ui(aux2, dados->y, 2);
	mpf_add(aux2, aux2, aux);
	mpf_mul(aux2, aux2, dados->y);
	
	mpf_set_d(aux3, 2);
	mpf_pow_ui(aux3, aux3, (2*dados->k)+3);
	mpf_mul(aux2, aux2, aux3);
	mpf_pow_ui(aux, aux, 4);
	mpf_mul(aux, aux, dados->a);
	mpf_sub(dados->a, aux, aux2);

	mpf_clear(aux);
	mpf_clear(aux2);
	mpf_clear(aux3);
	pthread_exit(arg);
}

void * calcula_y(void *arg){
	struct dados_threadY * dados = arg;

	mpf_t aux, aux2;
	mpf_init(aux);
	mpf_init(aux2);

	mpf_pow_ui(aux, dados->y_k2, 4);
	mpf_ui_sub(aux, 1.0, aux);
	mpf_sqrt(aux,aux);
	mpf_sqrt(aux,aux);
	mpf_ui_sub(aux2, 1.0, aux);
	mpf_add_ui(aux, aux, 1.0);
	mpf_div(dados->y, aux2, aux);

	mpf_clear(aux);
	mpf_clear(aux2);
	
	pthread_exit(arg);
}

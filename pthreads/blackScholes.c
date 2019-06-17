#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>

#define N_CORE 4

struct BoxMullerState
{
	double x1, x2, w, y1, y2;
	int useLast;
	struct drand48_data random;
};

void initBoxMullerState(struct BoxMullerState* state)
{
	state->random.__init = 0;
	state->useLast = 0;

	struct timeval now;
	gettimeofday(&now, NULL);
	state->random.__x[0] = now.tv_usec;
}

double boxMullerRandom(struct BoxMullerState* state)
{
	double randomNumber;
	
	if (state->useLast)
	{
		state->y1 = state->y2;
		state->useLast = 0;
	}
	else
	{
		do
		{
			drand48_r(&state->random, &state->x1);
			state->x1 = 2.0 * state->x1 - 1.0;
			drand48_r(&state->random, &state->x2);
			state->x2 = 2.0 * state->x2 - 1.0;
			state->w = state->x1 * state->x1 + state->x2 * state->x2;
		}
		while (state->w >= 1.0);
		state->w = sqrt((-2.0 * log(state->w)) / state->w);
		state->y1 = state->x1 * state->w;
		state->y2 = state->x2 * state->w;
		state->useLast = 1;
	}

	return state->y1;
}

struct dataThread
{
	double S, E, r, sigma, T;
	int M;
	double * result;
};

void * monteCarlo(void *arg){
	struct dataThread * dados = arg;
	dados->result = (double *) malloc((dados->M)/N_CORE * sizeof(double));
	struct BoxMullerState state;
	initBoxMullerState(&state);
	double t_aux, random_number;
	int i;
	for (i=0; i<(dados->M)/N_CORE; i++){
		random_number = boxMullerRandom(&state);
		t_aux = (dados->S)*exp((dados->T)*((dados->r)-0.5*(dados->sigma)*(dados->sigma)) + (dados->sigma)*sqrt(dados->T)*random_number);
		if(t_aux-(dados->E)<0){
			t_aux = 0;
		}
		dados->result[i] = exp((-1)*(dados->r)*(dados->T))*t_aux;
	}

	pthread_exit(arg);
}



int main(int argc, char *argv[]){
	if(argc!=3){
		printf("Número de entradas incorreto.\n");
		exit(1);
	}
	//abrir arquivo de variaveis
	FILE *fp;
	fp = fopen(argv[1], "r");
	if(fp == NULL){
		printf("Erro de abertura do arquivo.\n");
		exit(1);
	}
	double parametros[6];
	//double S, E, r, sigma, T, M;
	//salvar variaveis do arquivo
	fscanf(fp,"%lf", &parametros[0]);
	fscanf(fp,"%lf", &parametros[1]);
	fscanf(fp,"%lf", &parametros[2]);
	fscanf(fp,"%lf", &parametros[3]);
	fscanf(fp,"%lf", &parametros[4]);
	fscanf(fp,"%lf", &parametros[5]);
	fclose(fp);

	//abrir arquivo para salvar valores
	fp = fopen(argv[2], "w");
	if(fp==NULL){
		printf("Erro de abertura do arquivo.\n");
		exit(1);
	}

	pthread_t thread_id[N_CORE];
	void * thread_res;
	double thread[N_CORE];
	
	int i;
	//vetor com estruturas que serão passadas às threads
	struct dataThread data[N_CORE];

	for (i=0; i < N_CORE; i++){
		data[i].S = parametros[0];
		data[i].E = parametros[1];
		data[i].r = parametros[2];
		data[i].sigma = parametros[3];
		data[i].T = parametros[4];
		data[i].M = (int)parametros[5];
	}
	for (i=0; i<N_CORE; i++){
		thread[i] = pthread_create(&thread_id[i], NULL, monteCarlo, (void*)&data[i]);
	}
	for (i=0; i<N_CORE; i++){
		thread[i] = pthread_join(thread_id[i], &thread_res);	
	}
	//calcular média, desvio padrao etc
	double mean, std, confwidth, confmin, confmax;
	int j;
	mean = 0;
	for (i=0;i<N_CORE; i++){
		for (j=0; j<(data[i].M)/N_CORE; j++){
			mean = mean + data[i].result[j];
		}
	}
	mean = mean/parametros[5];
	std = 0;
	for (i=0;i<N_CORE; i++){
		for (j=0; j<(data[i].M)/N_CORE; j++){
			std = std + pow(data[i].result[j] - mean, 2.0);
		}
	}
	std = sqrt(std/parametros[5]);
	confwidth = 1.96*std/sqrt(parametros[5]);
	confmin = mean - confwidth;
	confmax = mean + confwidth;

	fprintf(fp, "S \t %lf\n", parametros[0]);
	fprintf(fp, "E \t %lf\n", parametros[1]);
	fprintf(fp, "r \t %lf\n", parametros[2]);
	fprintf(fp, "sigma \t %lf\n", parametros[3]);
	fprintf(fp, "T \t %lf\n", parametros[4]);
	fprintf(fp, "M \t %lf\n", parametros[5]);
	fprintf(fp, "Confidence interval: (%lf, %lf)", confmin, confmax);
	fclose(fp);
	return 0;
}
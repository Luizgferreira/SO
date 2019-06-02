#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <gmp.h>

#define N_ITER 1000000000

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
	
	mpf_t t_gmp, p_gmp, t_aux, pi, aux;

	mpf_init(t_gmp);
	mpf_init(p_gmp);
	mpf_init(t_aux);
	mpf_init(pi);
	mpf_init(aux);
	
	double a = 1.0;
	double a_ant;
	double b = 1/sqrt(2);

	mpf_set_d(t_gmp, 0.25);
	mpf_set_d(p_gmp, 1.0);
	
	for (int i=0; i<N_ITER; i++){
		a_ant = a;
		a = (a+b)/2;
		b = sqrt(a_ant*b);
		mpf_set_d(t_aux, pow(a_ant-a,2.0));
		mpf_mul(t_aux, t_aux, p_gmp);
		mpf_sub(t_gmp, t_gmp, t_aux);
		mpf_mul_ui(p_gmp, p_gmp, 2.0);
	}

	mpf_set_d(aux, pow(a+b,2)/4);
	mpf_div(pi, aux, t_gmp);
	mpf_out_str (fp, 10, 0, pi);

	return 0;
}
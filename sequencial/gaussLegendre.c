#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <gmp.h>

#define N_ITER 1000

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
	srand(time(NULL));
	
	mpf_t t_gmp, p_gmp, t_aux, pi, aux, a, a_ant, b;

	mpf_init2(p_gmp, 1000);
	mpf_init2(t_gmp, 1000);
	mpf_init2(t_aux, 1000);
	mpf_init2(pi, 1000);
	mpf_init2(aux, 1000);
	mpf_init2(a, 1000);
	mpf_init2(a_ant, 1000);
	mpf_init2(b, 1000);
	
	mpf_set_d(a, 1.0);
	mpf_set_d(b, 0.5);
	mpf_sqrt(b, b);


	mpf_set_d(t_gmp, 0.25);
	mpf_set_d(p_gmp, 1.0);
	
	for (int i=0; i<N_ITER; i++){
		mpf_set(a_ant, a);

		mpf_add(aux, a, b);
		mpf_div_ui(a, aux, 2);

		mpf_mul(aux, a_ant, b);
		mpf_sqrt(b, aux);

		mpf_sub(aux, a_ant, a);
		mpf_pow_ui(t_aux, aux, 2);

		mpf_mul(t_aux, t_aux, p_gmp);
		mpf_sub(t_gmp, t_gmp, t_aux);
		mpf_mul_ui(p_gmp, p_gmp, 2.0);
	}
	mpf_add(aux, a, b);
	mpf_pow_ui(aux, aux, 2);
	mpf_div_ui(aux, aux, 4.0);
	//mpf_set_d(aux, pow(a+b,2)/4);
	
	mpf_div(pi, aux, t_gmp);
	mpf_out_str (fp, 10, 0, pi);
	mpf_clear(t_gmp);
	mpf_clear(p_gmp);
	mpf_clear(t_aux);
	mpf_clear(pi);
	mpf_clear(aux);
	mpf_clear(a);
	mpf_clear(a_ant);
	mpf_clear(b);
	return 0;
}
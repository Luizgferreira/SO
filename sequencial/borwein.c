#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <gmp.h>

#define N_ITER 100000

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

	mpf_t a_gmp, y_gmp, aux, aux2;

	mpf_init(a_gmp);
	mpf_init(y_gmp);
	mpf_init(aux);
	mpf_init(aux2);

	mpf_set_d(y_gmp, sqrt(2)-1);
	mpf_set_d(a_gmp, 2*pow(sqrt(2)-1,2));

	for (int i=0;i<N_ITER;i++){
		mpf_pow_ui(aux, y_gmp, 4.0);
		mpf_ui_sub(aux, 1.0, aux);
		//mpf_pow_ui(aux, aux, 0.25);
		mpf_ui_sub(aux2, 1.0, aux);
		mpf_pow_ui(aux2, aux2, 0.25);
		mpf_ui_sub(aux2, 1.0, aux2);
		mpf_add_ui(aux, aux, 1.0);
		mpf_pow_ui(aux2, aux2, 0.25);
		mpf_ui_sub(aux2, 1.0, aux2);
		mpf_div(y_gmp, aux2, aux);

		mpf_add_ui(aux,y_gmp,1.0);
		mpf_pow_ui(aux2, y_gmp, 2.0);
		mpf_add(aux2, aux2, aux);
		mpf_mul(aux2, aux2, y_gmp);
		mpf_t aux3;
		mpf_init(aux3);
		mpf_set_d(aux3, 2);
		mpf_pow_ui(aux3, aux3, (2*i)+1);
		mpf_mul(aux2, aux2, aux3);
		mpf_pow_ui(aux, aux, 4.0);
		mpf_mul(aux, aux, a_gmp);
		mpf_sub(a_gmp, aux, aux2);
	}

	mpf_t pi;
	mpf_init(pi);
	mpf_ui_div(pi, 1.0, a_gmp);
	mpf_out_str (stdout, 10, 0, pi);

	return 0;
}

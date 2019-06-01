#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

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
	int m = 0;
	int i;
	double x, y, result;
	for(i = 0; i<N_ITER; i++){
		x = (double)rand()/(double)(RAND_MAX/1);
		y = (double)rand()/(double)(RAND_MAX/1);
		result = pow(x, 2.0) + pow(y, 2.0);
		if(result<1){
			m = m + 1;
		}
	}
	result = (double)m/N_ITER;
	fprintf(fp, "%lf\n", 4*result);
	fclose(fp);
	return 0;
}

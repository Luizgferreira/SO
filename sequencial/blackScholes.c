#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

double	randn (double mu, double sigma){
	double U1, U2, W, mult;
	static double X1, X2;
	static int call = 0;
	if (call == 1)
	{
		call = !call;
		return (mu + sigma * (double) X2);
	}
	do
	{
		U1 = -1 + ((double) rand () / RAND_MAX) * 2;
		U2 = -1 + ((double) rand () / RAND_MAX) * 2;
		W = pow (U1, 2) + pow (U2, 2);
	}
	while (W >= 1 || W == 0);
	mult = sqrt ((-2 * log (W)) / W);
	X1 = U1 * mult;
	X2 = U2 * mult;
	call = !call;
	return (mu + sigma * (double) X1);
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
	double S, E, r, sigma, T;
	int M;
	//salvar variaveis do arquivo
	fscanf(fp,"%lf", &S);
	fscanf(fp,"%lf", &E);
	fscanf(fp,"%lf", &r);
	fscanf(fp,"%lf", &sigma);
	fscanf(fp,"%lf", &T);
	fscanf(fp,"%d", &M);
	fclose(fp);
	//abrir arquivo para salvar valores
	fp = fopen(argv[2], "w");
	
	if(fp==NULL){
		printf("Erro de abertura do arquivo.\n");
		exit(1);
	}
	//realiza metodo de monte carlo
	srand(time(NULL));
	
	int i;
	double trials[M], t_aux, random_number, mean;
	mean = 0;
	
	for (i = 0; i<M; i++){
		//random_number = -1 + 2*(double)rand()/(double)(RAND_MAX/1);
		random_number = randn(0,1);
		t_aux = S*exp(T*(r-0.5*sigma*sigma) + sigma*sqrt(T)*random_number);
		if(t_aux-E<0){
			t_aux = 0;
		}
		trials[i] = exp((-1)*r*T)*t_aux;
		mean = mean + trials[i];
	}
	mean = mean/M;
	double std = 0;
	for (i=0;i<M;i++){
		std = std + pow(trials[i]-mean, 2.0);
	}
	std = sqrt(std/M);
	double confwidth = 1.96*std/sqrt(M);
	double confmin = mean - confwidth;
	double confmax = mean + confwidth;
	fprintf(fp, "S \t %lf\n", S);
	fprintf(fp, "E \t %lf\n", E);
	fprintf(fp, "r \t %lf\n", r);
	fprintf(fp, "sigma \t %lf\n", sigma);
	fprintf(fp, "T \t %lf\n", T);
	fprintf(fp, "M \t %d\n", M);
	fprintf(fp, "Confidence interval: (%lf, %lf)", confmin, confmax);
	fclose(fp);
	return 0;
}
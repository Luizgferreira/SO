#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#define N_ITER 

void main(){
	srand(time(NULL));
	int m = 0;
	int i;
	double x, y, result;
	for(i = 0; i<1000000000; i++){
		x = (double)rand()/(double)(RAND_MAX/1);
		y = (double)rand()/(double)(RAND_MAX/1);
		result = pow(x, 2.0) + pow(y, 2.0);
		if(result<1){
			m = m + 1;
		}
	}
	result = (double)m/i;
	printf("pi: %f\n", result*4);



}

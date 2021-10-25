/*
 * This is a test for dpolynom
 *
 */

#define MAINPROGRAM

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include <time.h>
#include "functions.h"
#include "geometry.h"
#include "global.h"
/*
 * randomize a polynom
 */


int main()
{
	time_t time_seconds = time(NULL);
	srand(time_seconds);

	int d = 1;
	int n = 100;
	rank = 5;

	double *x = malloc(sizeof(double) * n);
	double *coeff = malloc(sizeof(double) * rank );


	for(int k = 0; k < rank; k++){
		coeff[k] = (double) rand() / (double)  RAND_MAX;
		//printf("%f\n", coeff[k]);
	}

	double p_o, p_i, p_c, p_2;
	double h = 0.000001;

	printf("polynom:\tderivative by dpolynom\t derivative by forward\n");
	for(int k = 0; k < n; k++){
		x[k] = (double) rand() / (double) RAND_MAX;
		p_o = dpolynom(x[k], coeff, 0); 
		/* use internal derivation */
		p_i = dpolynom(x[k], coeff, d);	

		/* calculate derivation */
		p_2 = dpolynom(x[k] + h, coeff, 0);
		p_c = (p_2 - p_o) / h;
		printf("%f\t%f\t%f\n", p_o, p_i, p_c);
	}
	//printf("%i", rank);	
	free(x);
	free(coeff);
	return 0;
}




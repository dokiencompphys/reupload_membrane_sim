/*
 * This program calculates the volume of our initial condition, a pyramid,
 * effectively the sum of all lattice points and comapres it to
 * the analytical solution:
 *  VOL_pyramid = 1/3 * A_base * HEIGHT
 */


#define MAINPROGRAM

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "global.h"
#include "initial_cond.h"
#include "geometry.h"

int main()
{


	time_t time_seconds = time(NULL);
	srand(time_seconds);
	/* GLOBAL VARIABLES */
	/* ************ */
	N = (rand() % 20) + 5;
	D = 2;
	VOL = ipow(N, D);
	/* *********** */

	/* define the top of pyramid */
	int *coord_init = malloc(2 * sizeof(int));

	int RUNS = 5;
	double height = 1.0;

	double VOL_pyr_formula;
	double VOL_pyr_fun;
	double max_err = 0.0;
	double err[RUNS + 1];
	err[0] = 1.0;

	printf("N\tVOL_pyr_fun\tVOL_pyr_ana\trelative error\n");
	for(int t = 0; t < RUNS; t++) {
		while(coord_init[0] == 0 || coord_init[0] == N)
			coord_init[0] = rand() % N;
		while(coord_init[1] == 0 || coord_init[1] == N)
			coord_init[1] = rand() % N;

		VOL_pyr_formula = 0.0;
		VOL_pyr_fun = 0.0;

		for(int i = 0; i < VOL; i++)
		       VOL_pyr_fun += init_h_pyramid_a(coord_init, height)[i];
		VOL_pyr_formula = 1.0/3.0 * (N - 1) * (N - 1) * height;
		err[t + 1] = fabs(VOL_pyr_fun - VOL_pyr_formula)/VOL;

		if(err[t + 1] < err[t] ) {
			printf("%d\t%f\t%f\t%f\n", N , VOL_pyr_fun,
					VOL_pyr_formula, err[t + 1]);
			N += (rand() % 5) + 5;
			VOL = ipow(N,D);
		} else {
			printf(" test failed.");
			exit(-1);
		}

	}
	max_err = max_err / VOL;
	printf("The test has been succesful. The relative error is decreasing"
			" with N rising.\n"
			"The relative error between the volumina from"
			" the reconstructed pyramid to the analytical is: %f\n",
			err[RUNS]);

	free(coord_init);
	return 0;
}


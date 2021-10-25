/*
 * This program makes a testrun for leapfrog method. calculates energy
 * or starting conditions are pyramids
 *
 *
 */

#define MAINPROGRAM

#include <stdio.h>
#include <stdlib.h>

#include "geometry.h"
#include "initial_cond.h"
#include "integrators.h"
#include "global.h"
#include "observables.h"

int main()
{

	N = 10;
	D = 2;
	VOL = ipow(N,D);
	double ampl = 2.0;
	int time_steps = 10;

	double *h_arr = malloc(sizeof(double) * VOL);
	double *pi_arr = malloc(sizeof(double) * VOL);
	int *coord_init = malloc(sizeof(int) * 2);

	/* initialize coordinates and array  */
	for(int k = 0; k < 2; k++)
		coord_init[k] = 5;

	h_arr = init_h_pyramid_a(coord_init, ampl);
	for(int k = 0; k < VOL; k++)
		pi_arr[k] = 0;

	while(time_steps > 0) {
		leapfrog(h_arr, pi_arr);
		time_steps--;
	}

	for(int k = 0; k < VOL; k++)
		printf("%d\t%d", h_arr[k], pi_arr[k]);

	free(coord_init);
	free(pi_arr);
	free(h_arr);
	return 0;
}




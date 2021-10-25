#define MAINPROGRAM

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#include "global.h"
#include "geometry.h"
#include "functions.h"
#include "potential.h"
#include "integrators.h"
#include "initial_cond.h"
#include "observables.h"


int main(void)
{
	/* CARE GLOBAL VARIABLES */
	N = 21;
	D = 2;

	VOL = ipow(N, D);
	nn = nn_create();
	is_border_arr = is_border_create();
	/* ********************* */

	geo_print_info(stdout);

	pot_set_func(1);
	pot_set_param0(0.1);
	pot_print_info(stdout);

	integ_set_integrator(0);
	integ_tau = 0;
	integ_print_info(stdout);

	int *coord0 = malloc(2 * sizeof(int));
	coord0[0] = coord0[1] = N / 2;
	double ampl = 1.0;

	double *h_arr;
	double *pi_arr;

	double time_end = 12;
	double time = 0.0;
	int count = 0;

	double energy_t0 = 0.0;
	double energy_t = 0.0;
	double energy_diff = 0.0;

	double integ_tau_range[] = {0.5, 0.3, 0.2, 0.1, 0.05, 0.01, 0.005,
		0.004, 0.003, 0.002, 0.001};
	size_t integ_tau_range_size = sizeof(integ_tau_range)
		/ sizeof(integ_tau_range[0]);

	for (int i = 0; i < integ_tau_range_size; i++) {
		integ_tau = integ_tau_range[i];
		time = 0.0;
		count = 0;

		h_arr = init_h_pyramid_a(coord0, ampl);
		pi_arr = init_pi_zero();

		energy_t0 = hamilton(h_arr, pi_arr);

		while (time < time_end) {
			count++;
			time = count * integ_tau;

			integ_integrator(h_arr, pi_arr);
		}

		energy_t = hamilton(h_arr, pi_arr);
		energy_diff = energy_t0 - energy_t;

		printf("%e\t%e\n", integ_tau, energy_diff);

		free(h_arr);
		free(pi_arr);
	}

	return 0;
}

#define MAINPROGRAM

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#include "global.h"
#include "geometry.h"
#include "functions.h"


int main(int argc, char *argv[])
{
	/* CARE GLOBAL VARIABLES */
	N = 201;
	D = 2;

	if (argc > 0 && argc < 4) {
		if (argc == 2)
			D = atoi(argv[1]);
		if (argc == 3)
			N = atoi(argv[2]);
	} else {
		printf("ERROR: usage: test_function [D] [N]\n");
		exit(-1);
	}

	VOL = ipow(N, D);
	nn = nn_create();
	is_border_arr = is_border_create();
	/* ********************* */

	printf("INT_MAX = %d\n", INT_MAX);

	int seed = 20200307;
	srand(seed);

	/* is_border - counting test */
	/* TODO: getting the right formula */

	if (D == 2) {
		int border_count = 0;
		for (int ind = 0; ind < VOL; ind++)
			border_count += is_border(ind);

		int expected_border_count = (ipow(N, D - 1) - (D - 1)) * 2 * D;

		printf("[ is_border] counting test: successful, "
				"border_count = %d, expected_border_count = %d\n", border_count,
				expected_border_count);
		fflush(stdout);
	} else
		printf("[ is_border ] counting test: skipped "
				"because D != 2 (D = %d)\n", D);


	/* laplace_arr - linearity test */

	double *arr1 = malloc(sizeof(double) * VOL);
	double *arr2 = malloc(sizeof(double) * VOL);
	double *sup = malloc(sizeof(double) * VOL);
	double a = 2.1;
	double b = 1.2;
	for (int ind = 0; ind < VOL; ind++) {
		arr1[ind] = ((double) rand() / (double) RAND_MAX);
		arr2[ind] = ((double) rand() / (double) RAND_MAX);
		sup[ind] = a * arr1[ind] + b * arr2[ind];
	}


	double *lp_arr1 = malloc(sizeof(double) * VOL);
	double *lp_arr2 = malloc(sizeof(double) * VOL);
	double *lp_sup = malloc(sizeof(double) * VOL);

	laplace_arr(arr1, lp_arr1);
	laplace_arr(arr2, lp_arr2);
	laplace_arr(sup, lp_sup);

	double err = 0;
	for (int ind = 0; ind < VOL; ind++)
		err += fabs(lp_sup[ind] - (a * lp_arr1[ind] + b * lp_arr2[ind]));

	double err_density = err / VOL;
	double err_limit = 1e-15;

	if (err_density < err_limit) {
		printf("[ laplace_arr ] linearity test: sucessful, error density = %e"
				" < %e\n", err_density, err_limit);
	} else {
		printf("[ laplace_arr ] linearity test: failed, error density = %e"
				" > %e\n", err_density, err_limit);
		return 1;
	}

	return 0;
}

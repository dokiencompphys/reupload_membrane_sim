/*
 * This program tests the integrators to solve the differential equation
 *	(d/dt) v = - g; (d/dt) x = v;
 * analytically solved by:
 *	x = -g/2 * t^2 + v_0 * t + x_0;
 *	v = -g * t + v_0;
 *
 */

#define MAINPROGRAM

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "global.h"
#include "geometry.h"
#include "functions.h"
#include "potential.h"
#include "integrators.h"

int main(void)
{
	/* CARE GLOBAL VARIABLES */
	N = 1;
	D = 1;

	VOL = ipow(N, D);
	nn = nn_create();
	is_border_arr = is_border_create();
	/* ********************* */

	int runs = 30; /* points for x */

	double x_ana = 0.0;
	double v_ana = 0.0;
	double *x = malloc(sizeof(double));
	double *v = malloc(sizeof(double));

	x[0] = rand() / RAND_MAX;
	v[0] = fabs(rand()) / RAND_MAX;
	double v_0 = v[0];
	double x_0 = x[0];

	double T;
	double err_x = 0.0;
	double err_v = 0.0;
	double max_err_x = 0.0;
	double max_err_v = 0.0;

	/* choose newton's gravitation potential */
	double g_newt = 9.81;
	pot_set_param0(g_newt);
	pot_set_func(2);
	printf("with g = %.2f\tx_0 = %.2f\tv_0 = %.2f\n", g_newt, x_0, v_0);

	integ_tau = 0.01;

	printf("x\tx_ana\terr_x\terr_v\n");
	for(int i = 0; i < 2; i++) {
		integ_set_integrator(i);
		x[0] = x_0;
		v[0] = v_0;
		for(int n = 1; n < runs; n++) {
			T = integ_tau * n;
			integ_integrator(x, v);
			x_ana = -g_newt / 2.0 * T * T + v_0 * T + x_0;
			v_ana = -g_newt * T + v_0;
			err_x = fabs(x[0] - x_ana);
		        err_v =	fabs(v[0] - v_ana);
			printf("%f\t%f\t%e\t%e\n", x[0], x_ana, err_x, err_v);
			max_err_x = (err_x > max_err_x) ? err_x : max_err_x;
			max_err_v = (err_v > max_err_v) ? err_v : max_err_v;
		}

	}
	printf("the maximum error for x is %e\n"
			"the maximum error for v is %e\n",
			max_err_x, max_err_v);

	return 0;
}

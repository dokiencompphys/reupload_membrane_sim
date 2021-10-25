/*
 * symplectic integration methods
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "global.h"
#include "functions.h"
#include "potential.h"

void integ_is_not_set(double *dummy_a, double *dummy_b);
void integ_yoshida(double *h_arr, double *pi_arr);
void leapfrog(double *h, double *v);

void (*integ_integrator)(double *, double *) = &integ_is_not_set;

double integ_tau = 0.01;

static char info_select[300] = "[ integrator.c ]: invalid info string, "
"check integ_print_info";


/*
 * exits program if integ_integrator is not set
 */
void integ_is_not_set(double *dummy_a, double *dummy_b)
{
	printf("[ integrators.c]: integrator is not set\n");
	exit(-1);
}


/*
 * sets the integ_integrator with choice of integrator:
 * 0 for yoshida
 */
void integ_set_integrator(int select)
{
	if (select == 0) {
		integ_integrator = &integ_yoshida;
		strcpy(info_select, "yoshida integrator");
	} else if (select == 1) {
		integ_integrator = &leapfrog;
		strcpy(info_select, "leapfrog integrator");
	} else {
		printf("[ integratorcs.c : integ_set_integrator ]: selection of"
				" integrator is invalid: select = %d\n", select);
		exit(-1);
	}
}


/*
 * prints info about selected integrator
 */
void integ_print_info(FILE *stream)
{
	fprintf(stream, "# %s, integ_tau = %e\n", info_select, integ_tau);
}


/*
 *
 * calculates next time steps for initial space point, velocity
 * and accelaration
 */

void leapfrog(double *h, double *v)
{
        double *h_F = malloc(VOL * sizeof(double));
        double *a1 = malloc(VOL * sizeof(double));
        double a2 = 0;
        laplace_arr(h, h_F);

        /* calculate next time step */
        for(int n = 0; n < VOL; n++) {
                a1[n] = h_F[n] - pot_deriv_func(h[n]);
				//printf("a = %f\t", a1[n]);
				h[n] = h[n] + v[n] * integ_tau
					+ 0.5 * a1[n] * integ_tau * integ_tau;
        }
        /* necessary to through whole lattice before calculating lapalce */

        laplace_arr(h, h_F);

        /* update acceleraion and velocity*/
        /* TODO: find generalization for potential */
        for(int n = 0; n < VOL; n++) {
                /* a2 is acceleration for the next time step */
                a2 = h_F[n] - pot_deriv_func(h[n]);
				//printf("a2 = %f\n",a2);
                v[n] = v[n] + 0.5 * (a1[n] + a2) * integ_tau;
        }
        free(a1);
        free(h_F);
}


/* Yoshida 4th-order integrator */
/* TODO: optimization */
void integ_yoshida(double *h_arr, double *pi_arr)
{
	double c[4] = {0.6756035959798289,
		-0.17560359597982886,
		-0.17560359597982886,
		0.6756035959798289};

	double d[4] = {1.3512071919596578,
		-1.7024143839193155,
		1.3512071919596578,
		0.0};

	double *h_arr0 = malloc(sizeof(double) * VOL);
	double *h_arr1 = malloc(sizeof(double) * VOL);
	double *h_arr2 = malloc(sizeof(double) * VOL);
	double *pi_arr0 = malloc(sizeof(double) * VOL);
	double *pi_arr1 = malloc(sizeof(double) * VOL);
	double *pi_arr2 = malloc(sizeof(double) * VOL);
	double *lp_arr = malloc(sizeof(double) * VOL);

	for (int ind = 0; ind < VOL; ind++) {
		h_arr0[ind] = h_arr[ind] + c[0] * pi_arr[ind] * integ_tau;
	}
	laplace_arr(h_arr0, lp_arr);
	for (int ind = 0; ind < VOL; ind++) {
		pi_arr0[ind] = pi_arr[ind]
			+ d[0] * (lp_arr[ind] - pot_deriv_func(h_arr0[ind])) * integ_tau;
		h_arr1[ind] = h_arr0[ind] + c[1] * pi_arr0[ind] * integ_tau;
	}
	laplace_arr(h_arr1, lp_arr);
	for (int ind = 0; ind < VOL; ind++) {
		pi_arr1[ind] = pi_arr0[ind]
			+ d[1] * (lp_arr[ind] - pot_deriv_func(h_arr1[ind])) * integ_tau;
		h_arr2[ind] = h_arr1[ind] + c[2] * pi_arr1[ind] * integ_tau;
	}
	laplace_arr(h_arr2, lp_arr);
	for (int ind = 0; ind < VOL; ind++) {
		pi_arr2[ind] = pi_arr1[ind]
			+ d[2] * (lp_arr[ind] - pot_deriv_func(h_arr2[ind])) * integ_tau;

		h_arr[ind] = h_arr2[ind] + c[3] * pi_arr2[ind] * integ_tau;
		pi_arr[ind] = pi_arr2[ind];
	}

	free(h_arr0);
	free(h_arr1);
	free(h_arr2);
	free(pi_arr0);
	free(pi_arr1);
	free(pi_arr2);
	free(lp_arr);
}

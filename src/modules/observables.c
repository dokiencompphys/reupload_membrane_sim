#include <stdlib.h>

#include "global.h"
#include "potential.h"
#include "functions.h"


/*
 * hamilton function
 */
double hamilton(double *h, double *v)
{
	double res = 0;
	double *h_F = malloc(sizeof(double) * VOL);

	laplace_arr(h, h_F);
	for(int n = 0; n < VOL; n++) {
		res += 0.5 * v[n] * v[n]
			- 0.5 * h[n] * h_F[n] + pot_func(h[n]);
	}
	free(h_F);

	res = res/ ((N - 1) * (N - 1));
	return res;
}


#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "geometry.h"
#include "potential.h"

/*
 * IMPORTANT: in the main program has to be following line:
 * is_border_arr = is_border_create();
 * otherwise laplace_arr() will not work
 */


/*
 * Function: is_border
 * -------------------
 *  checks if index ind is a border point in coordinate space
 *     returns 1 if it is a border point
 *     returns 0 otherwise
 */
int is_border(int ind)
{
	int *coord = malloc(sizeof(int) * D);
	ind2coord(coord, ind);

	for (int d = 0; d < D; d++) {
		if (coord[d] == 0 || coord[d] == N - 1)
			return 1;
	}
	free(coord);
	return 0;
}


/*
 * Function: is_border_create
 * --------------------------
 *  creates precalculated array of is_border
 */
int *is_border_create(void)
{
	int *is_border_arr = malloc(sizeof(int) * VOL);

	for (int i = 0; i < VOL; i++)
		is_border_arr[i] = is_border(i);

	return is_border_arr;
}


/*
 * Function: laplace_arr
 * ---------------------
 *  computes the D-dimensional laplacian in index space of the wave function
 *     h''(r) = Δ h(r)
 *  but skips border points
 *
 *  *h: pointer to a double array[N^D]
 *        the input D-dimensional physical wave function in a 1-dimensional
 *        mapped complex double array format on which the hamiltonian act
 *
 *  **nn: array[D] of pointers to int arrays[2*N^D]
 *        the precalcuted nearest neighboar array, see nn_create() for details
 *
 *  *out: pointer to a double array[N^D]
 *        the output wave function after action of the laplacian on the
 *        input wave function
 *        ! gets overwritten !
 */
void laplace_arr(double *h, double *out)
{
	/* clearing for summation */
	for (int i = 0; i < ipow(N, D); i++)
		out[i] = 0;

	/* calculation of discretized laplacian in D-dimensions */
	for (int ind = 0; ind < ipow(N, D); ind++) {
		if (is_border_arr[ind]) {
			out[ind] = h[ind];
			continue;
		}
		for (int j = 0; j < D; j++) {
			out[ind] += h[nn[j][2 * ind]] - 2 * h[ind]
				+ h[nn[j][2 * ind + 1]];
		}
	}
}


/* calculates  (rank - 1) 'th ordered polynom   */
/*
 * d: order of derivative
 *
 * *coeff: pointer to an array with (rank) number of elements
 *
 * this produces (d/dx)^{d} p(x) = sum_{k = d}^{rank -1} a_k x^{k-d} * (k * (k-1) * ... * (k-d+1))
 *
 */
double dpolynom(double x, double *coeff, int d)
{
        double res = 0;
        double interim;
        int factor = 1;

        for(int r = 0; r < rank; r++) {
                interim = 0;
                factor = 1; /* reset to 1 */

                if(r < d)
                        interim = 0;
                else {
                        for(int i = 0; i < d; i++) /* if d == 0; give out polynom itself */
                                factor *= r - i;
                        interim = pow(x, r - d) * coeff[r] * factor;

                }
                res += interim;
        }
        return res;
}

/*
 * initial conditions for system
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "global.h"
#include "geometry.h"
#include "functions.h"


/*
 * all zero conditional condition h
 */
double *init_h_zero(void)
{
	double *out = malloc(sizeof(double) * VOL);
	for (int ind = 0; ind < VOL; ind++) {
		//out[ind] = 0.0;
		out[ind] = ((double) rand() / (double) RAND_MAX) - 0.5;
	}
	return out;

}


/*
 * makes a triangle without base
 * the top of the triangle is located at "top"
 */
double *triangle_a(double ampl, int top)
{
	double *vec = malloc(sizeof(double) * N);
	for(int k = 0; k < top + 1; k++) {	/* include top coordinate */
		vec[k] = ampl / top * k;
#ifdef DEBUG
		printf("vec[%i] = %f \n", k , vec[k]);
#endif
	}
	for (int k = top + 1; k < N - 1; k++) {
		/* why is it l - 1 and not l ? */
		vec[k] = -1 * ampl / ((N - 1) - top) * k
			+ ampl * (N - 1) / (N - 1 - top);
#ifdef DEBUG
		printf("vec[%i] = %f \n ", k , vec[k]);
#endif
	}
	vec[N - 1] = 0;
#ifdef DEBUG
	printf("N = %d, top = %d, ampl = %f", N, top, ampl);
#endif
	return vec;
}


/*
 * makes a triangle without base
 * the top of the triangle is located at "top"
 */
double *triangle_b(double ampl, int top)
{
	double *vec = malloc(sizeof(double) * N);
	/* left side of top func: f_left(n) = a * n + b */
	/* with a = ampl / top; b = 0.0 */
	for(int k = 0; k < top + 1; k++) {	/* include top coordinate */
		vec[k] = ampl / top * k;
#ifdef DEBUG
		printf("vec[%i] = %f \n", k , vec[k]);
#endif
	}
	/* right side of top func: f_right(n) = a * n + b */
	for (int k = top + 1; k < N; k++) {
		/* why is it l - 1 and not l ? */
				vec[k] = ampl / (top - N + 1.0) * (k - N + 1.0);
#ifdef DEBUG
		printf("vec[%i] = %f \n ", k , vec[k]);
#endif
	}
	vec[N - 1] = 0;
#ifdef DEBUG
	printf("N = %d, top = %d, ampl = %f", N, top, ampl);
#endif
	return vec;
}


/*
 * pyramid like pull for 2d classical field, edges are parallel to borders!
 * from 0, 1, ..., N
 *
 * *coord_init: pointer to initial coordinates where the membrane gets pulled
 *
 * ampl: double
 *
 */
double *init_h_pyramid_a(int *coord_init, double ampl)
{
	/* dimension checking */
	if(D != 2) {
		printf("[ initial_cond.c : init_h_pyramid_a ]"
				" for this inital condition the dimension has to be 2"
				" (D = %d)", D);
		exit(-1);
	}


	int *coord = malloc(2 * sizeof(int));
	double *x = malloc(N * sizeof(double));
	double *y = malloc(N * sizeof(double));
	double *vec = malloc(sizeof(double) * VOL);

	int ind = coord2ind(coord_init);
	if(is_border(ind)) {
		printf("Please choose starting coordinates from the insides!");
		exit(-1);
	}

	/* create triangle in y direction with "ampl" as height
	 * create triangle in x direction for correct scaling */
	x = triangle_a(ampl, coord_init[1]);
	y = triangle_a(ampl, coord_init[0]);

	/* fill in x direction with triangles */
	double *tri_sqbase_l = malloc(N * sizeof(double));
	double *tri_sqbase_r = malloc(N * sizeof(double));

	tri_sqbase_l = triangle_a(coord_init[1], coord_init[0]);
	for(int n = 0; n < N; n++)
		tri_sqbase_r[n] = N - 1 -
			triangle_a(N - 1 - coord_init[1], coord_init[0])[n];

	/* scale everything inside the base triangles  with x_triangle */

	ind = 0;
	/* rows */
	for(int r = 0; r < N; r++) {
		/* go through each element in row */
#ifdef DEBUG
		printf("row: %i\n", r);
#endif
		for(int c = 0; c < N; c++) {

			if(c < tri_sqbase_l[r] || c > tri_sqbase_r[r]) {
				vec[ind] = x[c];
#ifdef DEBUG
				printf("inside \n");
#endif
			} else {
				vec[ind] = y[r];
#ifdef DEBUG
				printf("outside\n");
#endif
			}
#ifdef DEBUG
			printf("c = %.2d\tl = %.2f\tr = %.2f\tvec[%d] = %.2f\n"
					,c , tri_sqbase_l[r],
				       tri_sqbase_r[r], ind, vec[ind]);
#endif
			ind++; /* cannot use coord2ind() since
		       it wouldnt deliver right indexing for this setup
		      from left to right: 0 1 2 3 4 ... */
		}
	}
	/* N^2 elements = VOL ? */

	free(tri_sqbase_l);
	free(tri_sqbase_r);
	free(coord);
	free(x);
	free(y);
	return vec;
}


/*
 * pyramid like pull for 2d classical field, edges are parallel to borders!
 * from 0, 1, ..., N
 *
 * *coord_init: pointer to initial coordinates where the membrane gets pulled
 *
 * ampl: double
 *
 */
double *init_h_pyramid_b(int *coord_init, double ampl)
{
	/* dimension checking */
	if(D != 2) {
		printf("[ initial_cond.c : init_h_pyramid_b ]"
				" for this inital condition the dimension has to be 2"
				" (D = %d)", D);
		exit(-1);
	}


	int *coord = malloc(2 * sizeof(int));
	double *x = malloc(N * sizeof(double));
	double *y = malloc(N * sizeof(double));
	double *vec = malloc(sizeof(double) * VOL);

	int ind = coord2ind(coord_init);
	if(is_border(ind)) {
		printf("Please choose starting coordinates from the insides!");
		exit(-1);
	}

	/* create triangle in x direction with "ampl" as height
	 * create triangle in y direction for correct scaling */
	x = triangle_b(ampl, coord_init[0]);
	y = triangle_b(1, coord_init[1]);

	/* h(x,y) = x*y_scale */
	for(int i = 0; i < VOL; i++) {
		ind2coord(coord, i);
		vec[i] = x[coord[0]] * y[coord[1]];
	}
	free(coord);
	free(x);
	free(y);
	return vec;
}

/*
 * all zero initial conditions pi field
 */
double *init_pi_zero(void)
{
	double *out = malloc(sizeof(double) * VOL);
	for (int ind = 0; ind < VOL; ind++)
		out[ind] = 0.0;
	return out;
}

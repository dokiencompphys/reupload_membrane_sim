#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "global.h" /* global external variables */

/*
int ipow(int base, int exp)
{
	if (exp < 0) {
		fprintf(stderr, "[ geometry.c : ipow ] Error!"
				" exponent has to be >0 (exp = %d)\n", exp);
		exit(1);
	}

	int ret = 1;

	while (exp-- > 0)
		ret *= base;

	return ret;
}
*/

/*
 * copied from "https://stackoverflow.com/questions/101439/the-most-efficient
 * -way-to-implement-an-integer-based-power-function-powint-int"
 */
int ipow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}


int *init_coord(void)
{
	int *coord = malloc(sizeof(int) * D);
	for (int i = 0; i < D; i++)
		coord[i] = 0;
	return coord;
}


/*
 * Function: coord2ind
 * -------------------
 * returns indices for given coordinates as we map a D-dimensional
 * object to a 1-D.
 *
 * *coord: pointer to array of integers (coordinates of point)
 *
 */
int coord2ind(int *coord)
{
	int ind = 0;
	for(int k = 0; k < D; k++)
		ind += coord[k] * ipow(N, k);
	return ind;
}


/*
 * Function:ind2coord
 * ---------------------
 * function which gives out coordinates to corresponding indices.
 * It is the reverse function of coord2ind.
 *
 * *coord: pointer to array of integers (coordinates of point)
 *
 * ind: integer, indicates which coordinates will come out
 */
void ind2coord(int *coord, int ind)
{
	if (ind < 0) {
		printf("[ geometry.c : ind2coord ]"
				" expecting index >= 0 but got ind = %d\n", ind);
		exit(-1);
	}

	int count = D - 1;

	while(count >= 0) {
		coord[count] = ind / ipow(N, count);
		ind = ind % ipow(N, count);

		count = count - 1;
	}
}


/*
 * Function: nneighbour
 * --------------------
 * returns index of nearest neighbour. Boundary conditions are respected:
 * non periodic boundary conditions
 *
 * ind_in: integer, initial index point
 *
 * axis: integer, indicates in which axis we look for the neighbour
 *	 axis[k] corresponds to look for coord[k+-1]
 *
 * direction: integer, indicates wether coord[k+1], or coord[k-1]
 *	      is being searched.
 *
 * returns -1 if the neighbour point is outside the boundary
 *
 */

/* non-zero e.g. 1(true) for positive direction
 * 0(false) for negative direction */
int nneighbour(int ind_in, int axis, int direction)
{
	int *coord_out, ind_out;
	coord_out = malloc(sizeof(int) * D);

	ind2coord(coord_out, ind_in);

	/* transform in lattice first */
	if (direction && coord_out[axis] != N - 1)
		coord_out[axis] = coord_out[axis] + 1;
		/* boundary condition */
	else if (!direction && coord_out[axis] != 0)
		coord_out[axis] = coord_out[axis] - 1;
	else
		return -1;

	/* transform back*/
	ind_out = coord2ind(coord_out);
	free(coord_out);

	return ind_out;
}


/*
 * Function: nn_create
 * -------------------
 * creates an pointer, which points to pointers. These pointers point
 * to arrays of different dimensions. Effectively having in each row alternating
 * between back (-) and forth (+) nearest neighbour of one lattice point.
 * In each column we specify another dimension.
 *
 */
int **nn_create(void)
{
	int **nn;
	nn = malloc(sizeof(int*) * D);

	for (int i = 0; i < D; i++)
		*(nn + i) = malloc(sizeof(int) * 2 * ipow(N, D));
		/* jump to the next block*/

	for (int i = 0; i < D; i++)
		for (int j = 0; j < ipow(N, D); j++) {
			nn[i][2 * j] = nneighbour(j, i, 0);
			nn[i][2 * j + 1] = nneighbour(j, i, 1);
		}

	return nn;
}


/*
 * prints geometric info to file stream
 */
void geo_print_info(FILE *stream)
{
	fprintf(stream, "# geometry: N = %d, D = %d\n", N, D);
}


/*
 * prints whole array in coordinate space
 */
void geo_print_arr(FILE *stream, double *arr)
{
	int *coord = malloc(sizeof(int) * D);

	for (int ind = 0; ind < VOL; ind++) {
		ind2coord(coord, ind);
		for (int d = 0; d < D; d++) {
			fprintf(stream, "%d\t", coord[d]);
		}
		fprintf(stream, "%.6e\n", arr[ind]);
	}

	fprintf(stream, "\n\n");

	free(coord);
}


/*
 * prints comment header for geo_print_arr
 */
void geo_print_arr_head(FILE *stream, char *unit)
{
	fprintf(stream, "# ");
	for (int d = 0; d < D; d++) {
		fprintf(stream, "coord_%d\t", d);
	}
	fprintf(stream, "%s\n", unit);
}

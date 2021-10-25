#ifndef MAINPROGRAM
	#define EXTERN extern
#else
	#define EXTERN
#endif

EXTERN int D;				/* number of space dimensions */
EXTERN int N;				/* number of lattice points in one dimension */
EXTERN int VOL;				/* volume = N^D */
EXTERN int **nn;			/* pointer to nearest neighbour array */
							/* see **nn_create() */
EXTERN int *is_border_arr;	/* pointer to array which tells if point is */
							/* border point, see *is_border_create() */
EXTERN int rank;			/* rank for potential */
EXTERN double a;			/* spacing */

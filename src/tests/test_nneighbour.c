#define MAINPROGRAM

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "global.h"
#include "geometry.h"


/* reminder: 0 is false, every other number is true */
int main()
{
	/* CARE GLOBAL VARIABLES */
	N = 100;
	D = 3;

	VOL = ipow(N, D);
	nn = nn_create();
	/* ********************* */

	/* test of nneighbour by checking the difference in each dimension
	 * between neighbours
	 */
	int *coord = init_coord();
	int *coord_n = init_coord();

#ifdef DEBUG
					/* DEBUG PRINT */
					printf("testing nneighbour:\n"
							"ind ind_n axisofneighbour axis direction"
							" difference\n");
#endif

	for (int ind = 0; ind < VOL; ind++) {	/* iteration over all indices */
		ind2coord(coord, ind);


		for (int d = 0; d < D; d++) {		/* axis of neighboar */

			for (int dir = 0; dir < 2; dir++) { /* bw=0/fw=1 direction */
				int ind_n = nneighbour(ind, d, dir);

				/* check if the neighbour should exist */
				if (ind_n == -1) {
					if ((coord[d] == 0 && dir!=0)
							|| (coord[d] == N - 1 && dir!=1)) {
						printf("[ geometry.c : nneighbour ]"
								" test: ind_n is -1 but it should not be"
								" outside the boundary (check DEBUG)\n");
						exit(-1);
					} else
					/* if it doesn't exist and shouldn't, continue next ind */
					/* it skips difference check then */
						continue;
				}

				ind2coord(coord_n, ind_n);

				/* check for correct difference between neighbours */
				for (int i = 0; i < D; i++) {
					int diff = coord[i] - coord_n[i];

#ifdef DEBUG
					/* DEBUG PRINT */
					printf("%d %d %d %d %s %d\n", ind, ind_n, d, i,
							dir ? "fw" : "bw", diff);
#endif

					if (i == d && (diff == (dir ? -1 : 1))) {
					} else if (i != d && diff == 0) {
					} else {
						printf("[ geometry.c : nneighbour ]"
								" test: comparing differences"
								" in coordinates of neighbours: failed\n");
						exit(-1);
					}
				}
			}
		}
	}

	/* free doesn't remove declared variables/pointers */
	/* actually there is no real way to remove them at all in c */
	free(coord);
	free(coord_n);

	printf("[ geometry.c : nneighbour ]"
			" test: comparing differences in coordinates"
			" of neighbours: successful\n");


	/* test of nn which is the product of **nn_create() by checking the
	 * difference of coordinates in each dimension between neighbours
	 */

#ifdef DEBUG
					/* DEBUG PRINT */
					printf("\n\ntesting nn_create:\n"
							"ind ind_n axisofneighbour axis direction"
							" difference\n");
#endif

	coord = init_coord();
	coord_n = init_coord();

	for (int ind = 0; ind < VOL; ind++) {	/* iteration over all indices */
		ind2coord(coord, ind);

		for (int d = 0; d < D; d++) {		/* axis of neighboar */
			for (int dir = 0; dir < 2; dir++) { /* bw=0/fw=1 direction */
				int ind_n = nn[d][2 * ind + dir];

				/* check if the neighbour should exist */
				if (ind_n == -1) {
					if ((coord[d] == 0 && dir!=0)
							|| (coord[d] == N - 1 && dir!=1)) {
						printf("[ geometry.c : nn_create ]"
								" test: ind_n is -1 but it should not be"
								" outside the boundary (check DEBUG)\n");
						exit(-1);
					} else
					/* if it doesn't exist and shouldn't, continue next ind */
					/* it skips difference check then */
						continue;
				}

				ind2coord(coord_n, ind_n);

				/* check for correct difference between neighbours */
				for (int i = 0; i < D; i++) {
					int diff = coord[i] - coord_n[i];

#ifdef DEBUG
					/* DEBUG PRINT */
					printf("%d %d %d %d %s %d\n", ind, ind_n, d, i,
							dir ? "fw" : "bw", diff);
#endif

					if (i == d && (diff == (dir ? -1 : 1))) {
					} else if (i != d && diff == 0) {
					} else {
						printf("[ geometry.c : nn_create ]"
								" test: comparing differences"
								" in coordinates of neighbours: failed\n");
						exit(-1);
					}
				}
			}
		}
	}

	free(coord);
	free(coord_n);

	printf("[ geometry.c : nn_create ]"
			" test: comparing differences in coordinates"
			" of neighbours: successful\n");

	return 0;
}


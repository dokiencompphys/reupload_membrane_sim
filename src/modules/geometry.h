#include <stdio.h>

int coord2ind(int *coord);
void ind2coord(int *coord, int ind);
int nneighbour(int ind_in, int axis, int direction);
int **nn_create(void);
int ipow(int base, int exp);
int *init_coord(void);
void geo_print_info(FILE *stream);
void geo_print_arr(FILE *stream, double *arr);
void geo_print_arr_head(FILE *stream, char *unit);

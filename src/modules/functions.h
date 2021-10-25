/*
 * IMPORTANT: in the main program has to be following line:
 * is_border_arr = is_border_create();
 * otherwise laplace_arr() will not work
 */

int is_border(int ind);
int *is_border_create(void);
void laplace_arr(double *h, double *out);
double dpolynom(double x, double *coeff, int d);

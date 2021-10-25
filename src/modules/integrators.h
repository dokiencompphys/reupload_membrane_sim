/*
 * IMPORTANT: in the main program has to be following line:
 * nn = nn_create();
 * is_border_arr = is_border_create();
 * otherwise the integrators will not work
 */


void integ_yoshida(double *h_arr, double *pi_arr);
void leapfrog(double *h, double *v);
void integ_set_integrator(int select);
void integ_print_info(FILE *stream);

extern void (*integ_integrator)(double *, double *);

extern double integ_tau; /* time step */

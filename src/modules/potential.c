#include <stdio.h>
#include <stdlib.h>
#include <string.h>


double pot_is_not_set(double dummy_a);
double pot_none(double dummy_a);
double pot_higgs(double h_n);
double pot_deriv_higgs(double h_n);
double pot_newt_grav(double h);
double pot_deriv_newt_grav(double h);

double (*pot_func)(double) = &pot_is_not_set;
double (*pot_deriv_func)(double) = &pot_is_not_set;

static double pot_param0 = 1.0;
static char info_select[300] = "[ potential.c ]: invalid info string, "
"check pot_print_info";


/*
 * exits program if pot_func and/or pot_deriv_func is not set
 */
double pot_is_not_set(double dummy_a)
{
	printf("[ potential.c ]: potential function is not set\n");
	exit(-1);
}


/*
 * sets the pot_func and pot_deriv_func with choice of functions:
 * 0 for no potential
 * 1 for higgs potential
 */
void pot_set_func(int select)
{
	if (select == 0) {
		pot_func = &pot_none;
		pot_deriv_func = &pot_none;
		strcpy(info_select, "no potential: V(h) = 0");
	} else if (select == 1) {
		pot_func = &pot_higgs;
		pot_deriv_func = &pot_deriv_higgs;
		strcpy(info_select, "higgs potential: V(h) = pot_param0 * h^4 / 4!");
	} else if (select == 2) {
		pot_func = &pot_newt_grav;
		pot_deriv_func = &pot_deriv_newt_grav;
		strcpy(info_select, "Newton's Gravitation field:"
				" V(h) = m * pot_param0 * h");
	} else {
		printf("[ potential.c : pot_set_pot ]: selection of potential function"
				" is invalid: select = %d\n", select);
		exit(-1);
	}
}


/* prints info as commentary */
void pot_print_info(FILE *stream)
{
	fprintf(stream, "# %s, pot_param0 = %e\n", info_select, pot_param0);
}


/*
 * sets parameter for the potential functions
 */
void pot_set_param0(double value)
{
	pot_param0 = value;
}


/*
 * zero potential
 */
double pot_none(double dummy_a)
{
		return 0;
}


/*
 * higgs potential
 */
double pot_higgs(double h_n)
{
	return pot_param0 * h_n * h_n * h_n * h_n / (2 * 3 * 4);
}


/*
 * derivative of higgs potential
 */
double pot_deriv_higgs(double h_n)
{
	return pot_param0 * h_n * h_n * h_n / (2 * 3);
}

/* TODO: fix info or potential */
/*
 * newton's gravitation potential
 */
double pot_newt_grav(double h)
{
	return h * h + pot_param0 * h;
}


/*
 * derivation newton's gravitation potential
 */
double pot_deriv_newt_grav(double h)
{
	return h + pot_param0;
}

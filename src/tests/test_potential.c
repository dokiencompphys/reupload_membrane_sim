#include <stdio.h>

#include "potential.h"

/* this is just a test of the concept with the potential.c structure */
int main(void)
{
	pot_set_func(0);
	pot_set_param0(1.0);
	pot_print_info(stdout);

	printf("x\tpot_func(x)\n");
	for (int h = 0; h < 20; h++) {
		double x = 0.1 * h;
		printf("%0.2f\t%f\n", x, pot_func(x));
	}

	printf("\n\nx\tpot_deriv_func(x)\n");
	for (int h = 0; h < 20; h++) {
		double x = 0.1 * h;
		printf("%0.2f\t%f\n", x, pot_deriv_func(x));
	}

	return 0;
}

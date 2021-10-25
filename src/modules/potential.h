#include <stdio.h>

void pot_set_param0(double value);
void pot_set_func(int select);
void pot_print_info(FILE *stream);

extern double (*pot_func)(double);
extern double (*pot_deriv_func)(double);

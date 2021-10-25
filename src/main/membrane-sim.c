# define MAINPROGRAM

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include "global.h"
#include "geometry.h"
#include "integrators.h"
#include "functions.h"
#include "potential.h"
#include "initial_cond.h"
#include "observables.h"

#define MAX_FILENAME_LEN 50

void empty_func(FILE* dummy0, char* dummy1){};
void print_help(void);

int main(int argc, char *argv[])
{
	/* CARE GLOBAL VARIABLES */
	N = 21;
	D = 2;
	/* ********************* */


	/* DEFAULT SETTINGS */
	int select_pot = 1; /* no potential */
	pot_set_func(select_pot);
	double val_param0 = 0.1;
	pot_set_param0(val_param0);

	int select_integ = 0; /* yoshida integrator */
	integ_set_integrator(select_integ);
	integ_tau = 0.01;

	int out_t = 1;	/* print every integrator loop */
	double time_end = 12.0;

	void (*print_arr_head)(FILE*, char*) = &geo_print_arr_head;
	void (*print_arr)(FILE*, double*) = &geo_print_arr;

	FILE *out_stream = stdout; /* standard output */
	FILE *out_stream_obs;	/* output for observables */

	char filename[MAX_FILENAME_LEN] = "";
	char filename_ext[MAX_FILENAME_LEN] = "";
	char filename_obs[MAX_FILENAME_LEN] = "";
	char filename_obs_ending[] = "_obs";
	/* **************** */


	/* COMMAND LINE HANDLING */
	int opt;
	char *ptr; /* pointer for checking conversion of strings to numerics */
	opterr = 1;
	while ((opt = getopt(argc, argv, "+N:D:p:w:i:t:T:f:e:h")) != -1) {
		switch (opt) {
			case 'N':
				/* N lattice points in each dimension */
				N = strtol(optarg, &ptr, 0);
#ifdef DEBUG
				printf("opt N: left over string after conv.: |%s|\n", ptr);
#endif
				if ((*ptr != '\0') | (N <= 0)) {
					printf("N = |%s| is not a valid positive integer\n",
							optarg);
					exit(-1);
				}
				break;

			case 'D':
				/* D dimensions */
				D = strtol(optarg, &ptr, 0);
#ifdef DEBUG
				printf("opt D: left over string after conv.: |%s|\n", ptr);
#endif
				if ((*ptr != '\0') | (D <= 0)) {
					printf("D = |%s| is not a valid positive integer\n",
							optarg);
					exit(-1);
				}

				break;
			case 'p':
				/* choice of potential */
				select_pot = strtol(optarg, &ptr, 0);
#ifdef DEBUG
				printf("opt p: left over string after conv.: |%s|\n", ptr);
#endif
				if ((*ptr != '\0') | (select_pot < 0)) {
					printf("p = |%s| is not a valid non-negative integer\n",
							optarg);
					exit(-1);
				}

				pot_set_func(select_pot);

				break;
			case 'w':
				/* set pot_param0 */
#ifdef DEBUG
				printf("opt w: left over string after conv.: |%s|\n", ptr);
#endif
				val_param0 = strtod(optarg, &ptr);

				if (*ptr != '\0') {
					printf("param0 = |%s| is not a valid double\n", optarg);
					exit(-1);
				}
				pot_set_param0(val_param0);

				break;
			case 'i':
				/* choice of integrator */
				select_integ = strtol(optarg, &ptr, 0);
#ifdef DEBUG
				printf("opt i: left over string after conv.: |%s|\n", ptr);
#endif
				if ((*ptr != '\0') | (select_integ < 0)) {
					printf("i = |%s| is not a valid non-negative integer\n",
							optarg);
					exit(-1);
				}

				integ_set_integrator(select_integ);

				break;
			case 't':
				/* integrator time step integ_tau */
				integ_tau = strtod(optarg, &ptr);
#ifdef DEBUG
				printf("opt t: left over string after conv.: |%s|\n", ptr);
#endif
				if ((*ptr != '\0') | (integ_tau <= 0)) {
					printf("t = |%s| is not a valid positive double\n",
							optarg);
					exit(-1);
				}

				break;
			case 'T':
				/* ending time of simulation time_end */
				time_end = strtod(optarg, &ptr);
#ifdef DEBUG
				printf("opt T: left over string after conv.: |%s|\n", ptr);
#endif
				if ((*ptr != '\0') | (time_end <= 0)) {
					printf("T = |%s| is not a valid positive double\n",
							optarg);
					exit(-1);
				}

				break;
			case 'f':

				/* set filename to write to */
				strcpy(filename, optarg);
				strcpy(filename_obs, filename);

				/* set filename_obs for observables */
				/* with properly handling filename extension */
				char *fn_obs_ext = strrchr(filename_obs, '.');
				if (fn_obs_ext == NULL)
					strcat(filename_obs, filename_obs_ending);
				else {
					strcpy(filename_ext, fn_obs_ext);
					strcpy(fn_obs_ext, filename_obs_ending);
					strcat(filename_obs, filename_ext);
				}

				break;
			case 'e':
				/* output (e)very out_t calculation */
				out_t = strtol(optarg, &ptr, 0);
#ifdef DEBUG
				printf("opt e: left over string after conv.: |%s|\n", ptr);
#endif
				if ((*ptr != '\0') | (out_t <= 0)) {
					printf("e = |%s| is not a valid positive integer\n",
							optarg);
					exit(-1);
				}

				break;
			case 'h':
				/* output help */
				print_help();
				exit(0);

				break;
			case '?':
				/* error case */
				exit(-1);
		}
	}
	/* ********************* */


	/* CARE CALCULATED GLOBAL VARIABLES */
	VOL = ipow(N, D);
	nn = nn_create();
	is_border_arr = is_border_create();
	/* ******************************** */


	/* STARTING CONDITIONS */
	/* pyramid  */
	int *coord_0 = init_coord();
	double ampl = 1.0;
	for(int d = 0; d < 2; d++)
		coord_0[d] = (N - 1) / 4;

	double *h_arr = init_h_pyramid_a(coord_0, ampl);
	double *pi_arr = init_pi_zero();
	/* ******************* */


	/* if filename is set then write to file */
	if (strcmp(filename, ""))
		out_stream = fopen(filename, "w");

	/* if filename_obs is set then write to file */
	if(strcmp(filename_obs, ""))
		out_stream_obs = fopen(filename_obs, "w");
	else
		/* else trash output into /dev/null */
		/* TODO: think of a better solution so it doesnt calc the obs at all */
		out_stream_obs = fopen("/dev/null", "w");


	/* set time */
	double time = 0.0;
	int count = 0;
	time = count * integ_tau;


	/* energy vars */
	double energy_t0 = 0.0;
	double energy_t = 0.0;
	double energy_diff = 0.0;

	energy_t0 = hamilton(h_arr, pi_arr);
	energy_t = energy_t0;
	energy_diff = energy_t0 - energy_t;


	/* coords and ind for observable h(coord), pi(coord) */
	int ind_obs;
	int *coord_obs = init_coord();

	for (int d = 0; d < D; d++)
		coord_obs[d] = (N - 1) / 2;
	ind_obs = coord2ind(coord_obs);


	/* print_arr_head = &empty_func; */

	/* print info */
	geo_print_info(out_stream);
	pot_print_info(out_stream);
	integ_print_info(out_stream);

	geo_print_info(out_stream_obs);
	pot_print_info(out_stream_obs);
	integ_print_info(out_stream_obs);


	/* print t=0 for array */
	print_arr_head(out_stream, "height");
	fprintf(out_stream, "\n\"T = %f\"\n", time);
	print_arr(out_stream, h_arr);


	/* print t=0 for observables */
	fprintf(out_stream_obs, "\n\nT\tenergy\tenergy_diff\t "
			"h_arr[%d,%d]\tpi_arr[%d,%d]\n",
			 coord_obs[0], coord_obs[1],
			coord_obs[0], coord_obs[1]);
	fprintf(out_stream_obs, "%.3f\t%e\t%.3e\t%e\t%e\n", time, energy_t,
			energy_diff, h_arr[ind_obs], pi_arr[ind_obs]);


	while (time < time_end) {
		count++;
		time = count * integ_tau;

		integ_integrator(h_arr, pi_arr);

		if (count % out_t == 0) {
			/* calc energy violation energy_diff */
			energy_t = hamilton(h_arr, pi_arr);
			energy_diff = energy_t0 - energy_t;
			fprintf(out_stream_obs, "%.3f\t%e\t%.3e\t%e\t%e\n", time, energy_t,
					energy_diff, h_arr[ind_obs], pi_arr[ind_obs]);


			fprintf(out_stream, "\"T = %f\"\n", time);
			print_arr(out_stream, h_arr);
		}
	}

	fclose(out_stream);
	fclose(out_stream_obs);

	free(coord_0);
	free(coord_obs);
	free(h_arr);
	free(pi_arr);

	return 0;
}


void print_help(void)
{
	char help_text[] =
"Usage: membrane-sim [options]...\n"
"\n"
"The following options are available:\n"
"    -N <integer>    number of lattice points in one dimension, positive"
" integer\n"
"    -D <integer>    number of dimensions, positive integer\n"
"    -i <integer>    choice of integrator:\n"
"                        0    yoshida integrator\n"
"                        1    leapfrog integrator\n"
"    -p <integer>    choice of potential:\n"
"                        0    no potential\n"
"                        1    higgs potential\n"
"    -w <double>     set param0 for use in potential\n"
"    -t <float>      time step tau for a single integration step\n"
"    -T <float>      ending time t_end\n"
"    -e <integer>    output observables and field arrays only every"
" <integer>th\n"
"                    time integration step\n"
"    -f <file>       write output field to \"<file>\" and observables to \n"
"                    \"<file>_obs\"\n"
"                    if the specified filename has an filetype extension"
" then\n"
"                    the resulting observable file will share the same"
" extension\n"
"                    e.g.    simulated_system.dat ->"
" simulated_system_obs.dat\n"
"                            membrane_sys -> membrane_sys_obs\n"
"                    if this option is not specified the program will print"
" the\n"
"                    field array data to stdout and omit printing"
" observables\n"
"    -h              show this help text\n";

	printf("%s", help_text);
}

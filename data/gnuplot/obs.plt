filename="../test.dat_obs"

set key autotitle columnheader
stats filename nooutput

set grid

plot filename u 1:4 w lp, \
		 filename u 1:5 w lp

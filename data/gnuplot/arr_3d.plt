filename="../yosh_cont_sys_N301_t0.005_e100.dat"

set key autotitle columnheader
stats filename nooutput

# care if changing the geometry
col=STATS_max_x / 4 + 1

set zrange [-1:1]
set dgrid3d col,col
set hidden3d

do for [IDX=(STATS_blocks-2):(STATS_blocks-2)] {
	splot filename u 1:2:3 i IDX w l
		pause 0.1
}

set terminal pngcairo size 960, 540 enhanced font 'Verdana, 12'

filename="../yosh_cont_sys_N401_t0.005_e10.dat"
OUT="../figs/animpng/"

set key autotitle columnheader
stats filename nooutput

title_N = "N = ".system("head ".filename." | awk '{ if ($0 ~ /N = /) print $5}' | tr -cd [:alnum:]")
title_tau = "τ = ".system("head ".filename." | awk '{if ($0 ~ /integ_tau = /) printf \"%.1e\", $NF}'")
set title "Initial Conditions for: ".title_N.", ".title_tau

col=STATS_max_x / 4 + 1 # use only a fourth of the points for the mesh
set zrange [-0:1]
set cbrange [-0:1]
set dgrid3d col,col
set surface
set hidden3d
set pm3d at b
set view 75, 115, 1, 1
set xlabel "coord[0]" rotate parallel
set ylabel "coord[1]" rotate parallel
set zlabel "Position h" rotate by 90

# -2 because of the empty lines at the end of file
do for [IDX=0:(STATS_blocks-2):10] {
	set output OUT."anim_".sprintf("%05d", IDX).".png"
	splot filename u 1:2:3 i IDX w l
	unset output
}

set terminal pngcairo size 1920, 1080 enhanced font 'Verdana,12'
OUT = "../figs/"

set output OUT.'obs_cont_sys_e.png'
set key top right
set grid
set mytics
set mxtics
set multiplot layout 3,3 title "Continuous System Approximation"
set xrange [*:*]

array arr_ylabel[3] = [ "Energy Violation ΔH", "Position h[(N-1)/2,(N-1)/2]", "Momentum pi[(N-1)/2,(N-1)/2]" ]

do for [col in "3 4 5"] {

if (col == 3) {
set logscale y
} else {
unset logscale
}

do for [tau in "0.015 0.01 0.005"] {
	filenames=system("ls ../yosh_cont_sys_*_t".tau."_e??_obs.dat")
		set ylabel arr_ylabel[col-2]
		set title 
		set grid
		filename(i) = word(filenames, i)
		title_N(i) = "N = ".system("head ".filename(i)." | awk '{ if ($0 ~ /N = /) print $5}' | tr -cd [:alnum:]")

		title_tau = "τ = ".system("head ".filename(1)." | awk '{if ($0 ~ /integ_tau = /) print $NF}'")
		set title title_tau

		plot for [i = 1:words(filenames)] filename(i) u 1:int(col) w l title title_N(i)
}

}

unset multiplot
unset output

set output OUT.'obs_cont_sys_e_zoom.png'
set key bottom left
set grid
set mytics
set mxtics
set multiplot layout 3,3 title "Continuous System Approximation"

set xrange [11.5:12]

array arr_ylabel[3] = [ "Energy Violation ΔH", "Position h[(N-1)/2,(N-1)/2]", "Momentum pi[(N-1)/2,(N-1)/2]" ]

do for [col in "3 4 5"] {

if (col == 3) {
set logscale y
} else {
unset logscale
}

do for [tau in "0.015 0.01 0.005"] {
	filenames=system("ls ../yosh_cont_sys_*_t".tau."_e??_obs.dat")
		set ylabel arr_ylabel[col-2]
		set title 
		set grid
		filename(i) = word(filenames, i)
		title_N(i) = "N = ".system("head ".filename(i)." | awk '{ if ($0 ~ /N = /) print $5}' | tr -cd [:alnum:]")

		title_tau = "τ = ".system("head ".filename(1)." | awk '{if ($0 ~ /integ_tau = /) print $NF}'")
		set title title_tau

		plot for [i = 1:words(filenames)] filename(i) u 1:int(col) w lp title title_N(i)
}

}

unset multiplot
unset output

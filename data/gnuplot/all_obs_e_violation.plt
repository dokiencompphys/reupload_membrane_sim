set terminal pngcairo size 960, 540 enhanced font 'Verdana,12'
set key bottom right
set grid
OUT="../figs/"

set logscale y
set mxtics
set xlabel "Time t"
set ylabel "Energy Conservation ΔH"

filenames = system("ls -r1 ../yosh_e_vio*_obs.dat")

#no_of_files = words(filenames)
#array filename[no_of_files]
#do for [ind = 1:no_of_files] {
#	filename[ind] = word(filenames, ind);
#	title_s = "tau =".system("awk '{if ($0 ~ /integ_tau =/) print $NF}' ".filename[ind])
#	print title_s
#}


set output OUT."all_obs_e_vio_yosh.png"

plot for [file in filenames] file u 1:3 w l title "τ =".system("awk '{if ($0 ~ /integ_tau =/) printf \"%.1e\", $NF}' ".file)

unset output


set output OUT."all_obs_e_vio_leap.png"

filenames = system("ls -r1 ../leap_e_vio*_obs.dat")
plot for [file in filenames] file u 1:3 w l title "τ =".system("awk '{if ($0 ~ /integ_tau =/) printf \"%.1e\", $NF}' ".file)

unset output


set output OUT."all_obs_e_vio_leap_fit.png"
filename="../leap_e_vio_extract.dat"
stats filename u 1:3 nooutput

a=1e6
b=2
f(x)=a*x**b
fit f(x) filename u 1:($3 / STATS_max_y) via a,b
titlestr_f = sprintf("f(x) = a * x^{b}", a, b)

#had to split sprintf because of bug?!
#doesnt escape % right if more arguments are given
#maybe report it later
label1str = "Fit Parameter:\n".sprintf("a = %E\na_{err} = %+f\%\n", a, a_err / a * 100).\
sprintf("b = %f\nb_{err} = %+f\%\n", b, b_err / b * 100)

set label 1 label1str at screen 0.15,0.85

unset logscale y
set xlabel "Timestep τ"
set ylabel "Energy Conservation ΔH(t=12)"
titlestr = "Leapfrog Integrator"
plot filename u 1:($3 / STATS_max_y) w lp title titlestr, f(x) title titlestr_f

unset label 1

unset output


set output OUT."all_obs_e_vio_yosh_fit.png"
filename="../yosh_e_vio_extract.dat" 
stats filename u 1:3 nooutput

a=1e8
b=4
f(x) = a*x**b
fit f(x) filename u 1:($3 / STATS_max_y) via a,b
titlestr_f = sprintf("f(x) = a * x^{b}", a, b)

label1str = "Fit Parameter:\n".sprintf("a = %E\na_{err} = %+f\%\n", a, a_err / a * 100).\
sprintf("b = %f\nb_{err} = %+f\%\n", b, b_err / b * 100)

set label 1 label1str at screen 0.15,0.85

unset logscale y
set xlabel "Timestep τ"
set ylabel "Energy Conservation ΔH(t=12)"
titlestr = "Yoshida Integrator"
plot filename u 1:($3 / STATS_max_y) w lp title titlestr, f(x) title titlestr_f

unset label 1

unset output

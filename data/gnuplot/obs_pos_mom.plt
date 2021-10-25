set terminal pngcairo size 960, 540 enhanced font 'Verdana,12'
set grid
OUT="../figs/"

set mxtics
set xlabel "Time t"

set key bottom left

#################################
filenames = system("ls -r1 ../yosh_pos_mom*_obs.dat")
set output OUT."obs_pos_yosh.png"
set title "Yoshida Integrator"
set ylabel "Position h[10,10]"

plot for [file in filenames] file u 1:4 w l title "τ =".system("awk '{if ($0 ~ /integ_tau =/) printf \"%.1e\", $NF}' ".file)

unset title
unset output


#################################
set output OUT."obs_mom_yosh.png"
set title "Leapfrog Integrator"
set ylabel "Momentum pi[10,10]"

plot for [file in filenames] file u 1:5 w l title "τ =".system("awk '{if ($0 ~ /integ_tau =/) printf \"%.1e\", $NF}' ".file)

unset title
unset output


#################################
filenames = system("ls -r1 ../leap_pos_mom*_obs.dat")
set output OUT."obs_pos_leap.png"
set title "Yoshida Integrator"
set ylabel "Position h[10,10]"

plot for [file in filenames] file u 1:4 w l title "τ =".system("awk '{if ($0 ~ /integ_tau =/) printf \"%.1e\", $NF}' ".file)

unset title
unset output


#################################
set output OUT."obs_mom_leap.png"
set title "Leapfrog Integrator"
set ylabel "Momentum pi[10,10]"

plot for [file in filenames] file u 1:5 w l title "τ =".system("awk '{if ($0 ~ /integ_tau =/) printf \"%.1e\", $NF}' ".file)

unset title
unset output


#################################
set output OUT."obs_pos_tau_comparison.png"
filename_yosh="../yosh_pos_mom_extract.dat"
filename_leap="../leap_pos_mom_extract.dat"

set key bottom right
set title "Comparison: Yoshida - Leapfrog Integrator"
set xlabel "Timestep τ"
set ylabel "Position h[10,10]"

f_yosh(x) = a_yosh*x**b_yosh+c_yosh
f_leap(x) = a_leap*x**b_leap+c_leap

a_yosh=1
b_yosh=1
c_yosh=1
fit f_yosh(x) filename_yosh u 1:4 via a_yosh,b_yosh,c_yosh
a_leap=-0.095
b_leap=2
c_leap=-0.8489
fit f_leap(x) filename_leap u 1:4 via a_leap,b_leap,c_leap

#had to split sprintf because of bug?!
#doesnt escape % right if more arguments are given
#maybe report it later
label1str = "Fit Parameter:\n".sprintf("a_{yosh} = %f\na_{yosh err} = %+f\%\n",\
 a_yosh, a_yosh_err / a_yosh * 100)\
.sprintf("b_{yosh} = %f\nb_{yosh err} = %+f\%\n", b_yosh, b_yosh_err / b_yosh * 100)\
.sprintf("c_{yosh} = %f\nc_{yosh err} = %+f\%\n", c_yosh, c_yosh_err / c_yosh * 100)

label2str = "Fit Parameter:\n".sprintf("a_{leap} = %f\na_{leap err} = %+f\%\n",\
 a_leap, a_leap_err / a_leap * 100)\
.sprintf("b_{leap} = %f\nb_{leap err} = %+f\%\n", b_leap, b_leap_err / b_leap * 100)\
.sprintf("c_{leap} = %f\nc_{leap err} = %+f\%\n", c_leap, c_leap_err / c_leap * 100)

set label 1 label1str at screen 0.15,0.85
set label 2 label2str at screen 0.45,0.85

titlestr_yosh = "Yoshida Integrator"
titlestr_leap = "Leapfrog Integrator"
titlestr_f_yosh = sprintf("f_{yosh}(x) = a_{yosh} * x^{b_{yosh}} + c_{yosh}")
titlestr_f_leap = sprintf("f_{leap}(x) = a_{leap} * x^{b_{leap}} + c_{leap}")
plot filename_yosh u 1:4 w lp title titlestr_yosh, filename_leap u 1:4 w lp title titlestr_leap,\
f_yosh(x) title titlestr_f_yosh, [:0.03] f_leap(x) title titlestr_f_leap

unset label 1
unset label 2
unset title

unset output


#################################
set output OUT."obs_mom_tau_comparison.png"
filename_yosh="../yosh_pos_mom_extract.dat"
filename_leap="../leap_pos_mom_extract.dat"

set key bottom left
set title "Comparison: Yoshida - Leapfrog Integrator"
set xlabel "Timestep τ"
set ylabel "Momentum pi[10,10]"

f_yosh(x) = a_yosh*x**b_yosh+c_yosh
f_leap(x) = a_leap*x**b_leap+c_leap

a_yosh=-1
b_yosh=4
c_yosh=-0.17
fit f_yosh(x) filename_yosh u 1:5 via a_yosh,b_yosh,c_yosh
a_leap=1
b_leap=2
c_leap=-0.17
fit f_leap(x) filename_leap u 1:5 via a_leap,b_leap,c_leap

#had to split sprintf because of bug?!
#doesnt escape % right if more arguments are given
#maybe report it later
label1str = "Fit Parameter:\n".sprintf("a_{yosh} = %f\na_{yosh err} = %+f\%\n",\
 a_yosh, a_yosh_err / a_yosh * 100)\
.sprintf("b_{yosh} = %f\nb_{yosh err} = %+f\%\n", b_yosh, b_yosh_err / b_yosh * 100)\
.sprintf("c_{yosh} = %f\nc_{yosh err} = %+f\%\n", c_yosh, c_yosh_err / c_yosh * 100)

label2str = "Fit Parameter:\n".sprintf("a_{leap} = %f\na_{leap err} = %+f\%\n",\
 a_leap, a_leap_err / a_leap * 100)\
.sprintf("b_{leap} = %f\nb_{leap err} = %+f\%\n", b_leap, b_leap_err / b_leap * 100)\
.sprintf("c_{leap} = %f\nc_{leap err} = %+f\%\n", c_leap, c_leap_err / c_leap * 100)

set label 1 label1str at screen 0.45,0.85
set label 2 label2str at screen 0.75,0.85

titlestr_yosh = "Yoshida Integrator"
titlestr_leap = "Leapfrog Integrator"
titlestr_f_yosh = sprintf("f_{yosh}(x) = a_{yosh} * x^{b_{yosh}} + c_{yosh}")
titlestr_f_leap = sprintf("f_{leap}(x) = a_{leap} * x^{b_{leap}} + c_{leap}")
plot filename_yosh u 1:5 w lp title titlestr_yosh, filename_leap u 1:5 w lp title titlestr_leap,\
f_yosh(x) title titlestr_f_yosh, [:0.03] f_leap(x) title titlestr_f_leap

unset label 1
unset label 2
unset title

unset output

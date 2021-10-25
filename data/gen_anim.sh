#!/bin/sh

cd "./gnuplot" \
&& gnuplot "arr_3d_gif.plt" \
&& cd "../figs/animpng" \
&& convert -delay 5 -loop 0 "anim_*.png" "../anim.gif" \
&& rm anim_*.png

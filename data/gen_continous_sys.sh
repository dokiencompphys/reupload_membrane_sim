#!/bin/sh

BIN="../executables/membrane-sim"
OUT="./"
PREFIX="cont_sys"

OPT_Y="-D 2 -p 1 -w 0.1 -i 0 -T 12"

#only use odd N values

 #hand picked tau and -e values
 ev=10
 for tau in 0.015 0.01 0.005; do
	 for N in $(seq 51 50 401);do
		 printf 'yosh; N = %d\ttau = %f\tev = %d\n' "$N" "$tau" "$ev"
		 ./$BIN $OPT_Y -N "$N" -e "$ev" -t "$tau" -f "$OUT/yosh_${PREFIX}_N${N}_t${tau}_e${ev}.dat"
	 done
 done

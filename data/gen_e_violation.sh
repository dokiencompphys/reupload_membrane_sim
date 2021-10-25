#!/bin/sh

BIN="../executables/membrane-sim"
OUT="./"

OPT_Y="-D 2 -N 21 -p 1 -w 0.1 -i 0 -T 12"
OPT_L="-D 2 -N 21 -p 1 -w 0.1 -i 1 -T 12"

# auto generated tau and -e values
#for tau in $(seq 0.01 -0.001 0.001); do
#	ev=$(echo "0.1 / $tau" | bc)
#	printf 'yosh; tau = %f\tev = %d\n' "$tau" "$ev"
#	./$BIN $OPT_Y -e "$ev" -t "$tau" -f "$OUT/yosh_t${tau}_e${ev}.dat"
#done
#
#for tau in $(seq 0.001 -0.0001 0.0001); do
#	ev=$(echo "0.1 / $tau" | bc)
#	printf 'leap; tau = %f\tev = %d\n' "$tau" "$ev"
#	./$BIN $OPT_L -e "$ev" -t "$tau" -f "$OUT/leap_t${tau}_e${ev}.dat"
#done

# hand picked tau and -e values
for tau in 0.01 0.008 0.0075 0.006 0.005 0.004 0.003 0.002 0.001; do
	ev=10
	printf 'yosh; tau = %f\tev = %d\n' "$tau" "$ev"
	./$BIN $OPT_Y -e "$ev" -t "$tau" -f "$OUT/yosh_e_vio_t${tau}_e${ev}.dat"
done

for tau in 0.001 0.0008 0.00075 0.0006 0.0005 0.0004 0.0003 0.0002 0.0001; do
	ev=100
	printf 'leap; tau = %f\tev = %d\n' "$tau" "$ev"
	./$BIN $OPT_L -e "$ev" -t "$tau" -f "$OUT/leap_e_vio_t${tau}_e${ev}.dat"
done

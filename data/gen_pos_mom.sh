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
for tau in 0.1 0.08 0.075 0.06 0.05 0.04 0.03 0.02 0.01; do
	ev=1
	printf 'yosh; tau = %f\tev = %d\n' "$tau" "$ev"
	./$BIN $OPT_Y -e "$ev" -t "$tau" -f "$OUT/yosh_pos_mom_t${tau}_e${ev}.dat"
done

for tau in 0.020 0.015 0.010 0.008 0.0075 0.0060 0.0050 0.0040 0.0030 0.0020 0.0010; do
	ev=10
	printf 'leap; tau = %f\tev = %d\n' "$tau" "$ev"
	./$BIN $OPT_L -e "$ev" -t "$tau" -f "$OUT/leap_pos_mom_t${tau}_e${ev}.dat"
done

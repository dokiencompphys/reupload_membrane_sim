#!/bin/sh

Time=12


out_file=leap_pos_mom_extract.dat

printf "tau\tT\tenergy_diff\tpos\tmom\n" > "$out_file"

for file in $(ls -1 leap_pos_mom*_obs.dat)
do
	awk -v "Time=$Time" '{if ($0 ~ /integ_tau = /) tau=$NF; if ($1 == Time) print tau, $1, $3, $4, $5}' \
		"$file" >> "$out_file"
done


out_file=yosh_pos_mom_extract.dat

printf "tau\tT\tenergy_diff\tpos\tmom\n" > "$out_file"

for file in $(ls -1 yosh_pos_mom*_obs.dat)
do
	awk -v "Time=$Time" '{if ($0 ~ /integ_tau = /) tau=$NF; if ($1 == Time) print tau, $1, $3, $4, $5}' \
		"$file" >> "$out_file"
done

#!/bin/sh

Time=12


out_file=leap_e_vio_extract.dat

printf "tau\tT\tenergy_diff\tpos\tmom\n" > "$out_file"

for file in $(ls -1 leap_e_vio*_obs.dat)
do
	awk -v "Time=$Time" '{if ($0 ~ /integ_tau = /) tau=$NF; if ($1 == Time) print tau, $1, $3, $4, $5}' \
		"$file" >> "$out_file"
done


out_file=yosh_e_vio_extract.dat

printf "tau\tT\tenergy_diff\tpos\tmom\n" > "$out_file"

for file in $(ls -1 yosh_e_vio*_obs.dat)
do
	awk -v "Time=$Time" '{if ($0 ~ /integ_tau = /) tau=$NF; if ($1 == Time) print tau, $1, $3, $4, $5}' \
		"$file" >> "$out_file"
done

#!/bin/bash

benchmark=canneal
directories=$(find . -type d -name "*_results" )

rates_path=$1
rates=$(<${rates_path})

for f in scripts/*.runconf
do
	filename=$(basename "$f")
	extension="${filename##*.}"
	filename="${filename%.*}"

	for d in $directories; do
		base_d=$(basename "$d")
		for r in $rates; do
			b=${d}/$r/bin
            input=$(<${f})

			{
			time ./${b}/${benchmark}_$r.out ${input} > output/${benchmark}_${base_d}_${filename}_${r}.txt
			gprof ./${b}/${benchmark}_$r.out gmon.out
			python eval.py output/${benchmark}_${base_d}_${filename}_0.0.txt output/${benchmark}_${base_d}_${filename}_${r}.txt
			} &>> ${benchmark}_${base_d}_${filename}_${r}.log
		done
	done
done	
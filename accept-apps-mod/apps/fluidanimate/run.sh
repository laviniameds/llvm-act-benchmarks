#!/bin/bash

benchmark=fluidanimate
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
			time ./${b}/${benchmark}_$r.out 1 1 $input output/${benchmark}_${base_d}_${filename}_${r}.fluid
			echo "INPUTS: 100 100"
			gprof ./${b}/${benchmark}_$r.out gmon.out
			python eval.py output/${benchmark}_${base_d}_${filename}_0.0.fluid output/${benchmark}_${base_d}_${filename}_${r}.fluid
			} &>> ${benchmark}_${base_d}_${filename}_${r}.log
		done
	done
done	
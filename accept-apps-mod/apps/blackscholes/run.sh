#!/bin/bash

benchmark=blackscholes
directories=$(find . -type d -name "*_results" )

rates_path=$1
rates=$(<${rates_path})

for f in test.data/input/*.data
do
	filename=$(basename "$f")
	extension="${filename##*.}"
	filename="${filename%.*}"

	for d in $directories; do
		base_d=$(basename "$d")
		for r in $rates; do
			b=${d}/$r/bin
            input=$f
			
			{
			time ${b}/${benchmark}_$r.out 1 $input test.data/output/${base_d}_${filename}_${r}.data
			gprof ./${b}/${benchmark}_$r.out gmon.out
			python3.8 eval.py test.data/output/${filename}_orig.data test.data/output/${base_d}_${filename}_${r}.data
			} &> ${benchmark}_${base_d}_${filename}_${r}.log
		done
	done
done	
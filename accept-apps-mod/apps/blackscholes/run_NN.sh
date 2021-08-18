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
			echo $b=${d}/$r/bin/
			b=${d}/$r/bin
	
			./bin/${benchbmark}.out ${f} in_16.txt
			./${b}/${benchmark}_$r.out ${f} test.data/output/${filename}_${benchmark}_${r}.data	
			{
			python ./scripts/qos.py test.data/output/${filename}_${benchmark}_orig.data test.data/output/${filename}_${benchmark}_${r}.data
			} &> ${benchmark}_${base_d}_${r}.log
		done
	done
done	
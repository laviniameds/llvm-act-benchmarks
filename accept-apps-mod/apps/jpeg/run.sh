#!/bin/bash

benchmark=jpeg
directories=$(find . -type d -name "*_results" )

rates_path=$1
rates=$(<${rates_path})

for f in test.data/rgb_input/*.rgb
do
	filename=$(basename "$f")
	extension="${filename##*.}"
	filename="${filename%.*}"

	for d in $directories; do
		base_d=$(basename "$d")
		for r in $rates; do
			b=${d}/$r/bin
            input=${f}

            mkdir -p test.data/output/${r}
			mkdir -p results/${filename}
			{
			time ./${b}/${benchmark}_$r.out $input test.data/output/${r}/${base_d}_${filename}_${r}.jpeg
			gprof ./${b}/${benchmark}_$r.out gmon.out
			python3.8 eval.py test.data/output/0.0/${base_d}_${filename}_0.0.jpeg test.data/output/${r}/${base_d}_${filename}_${r}.jpeg
			} &> results/$filename/${benchmark}_${base_d}_${filename}_${r}.log
		done
	done
done	
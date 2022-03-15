#!/bin/bash

benchmark=jpeg

red='\033[0;31m'
blue='\033[0;34m'
green='\033[0;32m'
nc='\033[0m' # No Color

rates_path=$1
rates=$(<${rates_path})

directories=$(find . -type d -name "*_results" )

for f in test.data/input/*.rgb
do
	filename=$(basename "$f")
	extension="${filename##*.}"
	filename="${filename%.*}"

	for d in $directories; do
		base_d=$(basename "$d")
		for r in $rates; do
			#echo $b=${d}/$r/bin/
			b=${d}/$r/bin
	
			#./bin/${benchmark}.out ${f} test.data/output/${filename}_${benchmark}_orig.rgb
			./${b}/${benchmark}_$r.out ${f} test.data/output/${filename}_$f_${benchmark}_${r}.jpeg
			{
			# python3.8 ../../scripts/png2rgb.py png test.data/output/${filename}_orig.rgb test.data/output/${filename}.rgb_orig.png > out1_${filename}_${benchmark}_orig.tmp
			# python3.8 ../../scripts/png2rgb.py png test.data/output/${filename}_${benchmark}_${r}.rgb test.data/output/${filename}_${benchmark}_orig.png > out2_${filename}_${benchmark}_${r}.tmp
			# compare -metric RMSE test.data/output/${filename}.rgb_orig.png test.data/output/${filename}_${r}.png null > tmp_${filename}_${r}.log 2> tmp_${filename}_${r}.err
			echo -ne "${red}- $f\t"
			awk '{ printf("*** Error: %0.2f%\n",substr($1, 2, length($1) - 2) * 100) }' tmp_${filename}_${r}.err
			echo -ne "${nc}"						
			} &> ${benchmark}_${base_d}_${r}.log
		done
	done
done


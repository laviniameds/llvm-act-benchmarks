#!/bin/bash

benchmark=streamcluster
directories=$(find . -type d -name "*_results" )

rates_path=$1
rates=$(<${rates_path})

# filename=$( basename "$f")
# extension="${filename##*.}"
# filename="${filename%.*}"

for d in $directories; do
    base_d=$(basename "$d")
    for r in $rates; do
        b=${d}/$r/bin
        # input=$(<${f})

        # ${b}/${benchmark}_${r}.out 2 5 1 1 1 5 none output/${benchmark}_${base_d}_${r}.txt 1

        # {
        # python3.8 eval.py output/${benchmark}_${base_d}_0.0.txt output/${benchmark}_${base_d}_${r}.txt
        # } &>> ${benchmark}_${base_d}_${r}.log
        {
            time ./${b}/${benchmark}_$r.out 2 5 1 5 1 5 none output/${benchmark}_${base_d}_${filename}_${r}.txt 1
            gprof ./${b}/${benchmark}_$r.out gmon.out
            python3.8 eval.py output/${benchmark}_${base_d}_${filename}_0.0.txt output/${benchmark}_${base_d}_${filename}_${r}.txt
        } &>> ${benchmark}_${base_d}_${filename}_${r}.log
    done
done	
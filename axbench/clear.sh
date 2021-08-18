#!/bin/bash
find $1 -name "*.log" -type f -delete
find $1 -name "*.err" -type f -delete
find $1 -name "*.tmp" -type f -delete

# get all files in dir 
# files=$(find $1 -type f \( -iname \*.ll -o -iname \*.txt -o -iname \*.err -o -iname \*.log \))

# for src in $files; do
#     rm $src
# done
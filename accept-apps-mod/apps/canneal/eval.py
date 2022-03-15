#!/usr/bin/python

from __future__ import division
import sys

file1 = open(sys.argv[1])
file2 = open(sys.argv[2])
for line in file1:
    f1 = line.strip().split()
for line in file2:
    f2 = line.strip().split()  

    if (f1[0] == '0'):
        orig = 1
    else:
        orig = float(f1[0])
    if (f2[0] == '0'):
        relaxed = 1
    else:
        relaxed = float(f2[0])
    print("orig: {0:.20f} - relaxed: {1:.20f}").format(orig, relaxed)
    if(orig != relaxed):
        error = relaxed/orig
        print("error: {0}").format(abs(error))
    else:
        print("error: {0}").format(0)

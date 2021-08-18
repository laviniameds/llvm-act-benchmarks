#!/usr/bin/python

from __future__ import division
import sys

file = open(sys.argv[1])
for line in file:
    f = line.strip().split()
    if (f[0] == '0'):
        orig = 1
        relaxed = 1
    else:
        orig = float(f[0])
        relaxed = float(f[0])
    print("error: ", abs(relaxed/orig))

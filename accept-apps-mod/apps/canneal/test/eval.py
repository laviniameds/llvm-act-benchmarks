from __future__ import division

file = open('output.txt')
for line in file:
    f = line.strip().split()
    orig = 132
    relaxed = float(f[0])
    print(abs(relaxed / orig - 1.0))

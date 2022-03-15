from __future__ import division
import subprocess
import re
import os
import sys

def load():
    return 'file:out.fluid'

MISMATCH_RE = r'Expected <([-\.\de,]+)>\s+Received <([-\.\de,]+)>'

def euclidean_dist(a, b):
    if len(a) != len(b):
        # Mismatch in vector length. Maximal error.
        return len(a) ** 0.5

    total = 0.0
    for x, y in zip(a, b):
        total += (x - y) ** 2.0
    return total ** 0.5

def score(orig, relaxed):
    output, _ = subprocess.Popen([
        os.path.join(os.path.dirname(__file__), './fluidcmp'), relaxed, orig,
        '--verbose', '--ptol', '0',
    ], stdout=subprocess.PIPE).communicate()

    #print(output)
    num_particles = int(re.search(r'numParticles: (\d+)', output).group(1))

    total_dist = 0.0
    for expected, received in re.findall(MISMATCH_RE, output):
        evec = []
        rvec = []
        for s, vec in ((expected, evec), (received, rvec)):
            vec[:] = map(float, s.split(','))
        total_dist += euclidean_dist(evec, rvec)

    print("total_dist: " + str(total_dist))
    
    formatted_float = "{:.15f}".format(total_dist / (3 ** 0.5) / num_particles)

    return ("score error: " + str(formatted_float))

# For testing.
#if __name__ == '__fluidcmp__':
print (score(sys.argv[1], sys.argv[2]))

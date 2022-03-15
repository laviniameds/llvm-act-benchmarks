import munkres
import math

def load(fn='output.txt'):
    centers = []

    with open(fn) as f:
        while True:
            ident = f.readline()
            if not ident:
                break
            weight = f.readline()
            coords = f.readline()
            f.readline()

            centers.append((int(ident), float(weight),
                            map(float, coords.split())))

    return centers

def euclidean_dist(a, b):
    if len(a) != len(b):
        # Mismatch in vector length. Maximal error.
        return len(a) ** 0.5

    total = 0.0
    for x, y in zip(a, b):
        total += (x - y) ** 2.0
    return total ** 0.5

def _drop_nans(v):
    out = []
    for n in v:
        print("n: {0}", n)
        if math.isnan(n):
            out.append(0.0)
        else:
            out.append(n)
    print("out: {0}", out)
    return out

def score(orig, relaxed):
    matrix = []
    for _, _, a in orig:
        a = _drop_nans(a)
        row = []
        for _, _, b in relaxed:
            b = _drop_nans(b)
            row.append(euclidean_dist(a, b))
        matrix.append(row)
    indices = munkres.Munkres().compute(matrix)
    for i, j in indices:
        a = _drop_nans(orig[i][2])
        b = _drop_nans(relaxed[j][2])
        print(a)
        print("\n")
        print(b)
        dist = euclidean_dist(a, b)
        dist /= len(a) ** 0.5
        total += dist

    if len(orig) > len(relaxed):
        for i in range(len(orig) - len(relaxed)):
            total += 1

    return total / len(orig)

if __name__ == '__main__':
    import sys
    print(score(load(sys.argv[1]), load(sys.argv[2])))

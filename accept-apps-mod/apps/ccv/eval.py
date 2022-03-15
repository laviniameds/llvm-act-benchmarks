import math


def load():
    out = []
    with open('my_output.txt') as f:
        for line in f:
            out.append(line)
    return out


def score(orig, relaxed):
    total = 0.0
    diagonal = math.sqrt(math.pow(2400, 2) + math.pow(1745, 2))

    for a, b in zip(orig, relaxed):
        # Each line contains a pair of coordinates.
        x_a, y_a = a.split(None, 1)
        x_b, y_b = b.split(None, 1)
        x_a = float(x_a)
        y_a = float(y_a)
        x_b = float(x_b)
        y_b = float(y_b)

        # Euclidean distance.
        distance = math.sqrt(math.pow((x_b - x_a), 2) +
                             math.pow((y_b - y_a), 2))
        total += min(distance / diagonal, 1.0)

    # If any values are missing from the output, charge a maximal error
    # (1.0) for each missing element.
    if len(orig) > len(relaxed):
        total += len(orig) - len(relaxed)

    return total / len(orig)

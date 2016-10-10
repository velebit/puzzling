#!/usr/bin/python3

import math

def num_paths(n, r):
    fr = math.factorial(r)
    return math.factorial(n) / (math.factorial(n-2*r) * fr * fr)

total = 0
for n in (4, 6):
    n_total = 0
    for r in range(int(n/2) + 1):
        p = num_paths(n, r)
        print("n=%d r=%d   paths: %d" % (n, r, p))
        n_total += p
    print("n=%d total paths: %d" % (n, n_total))
    total += n_total
print("total     paths: %d" % total)

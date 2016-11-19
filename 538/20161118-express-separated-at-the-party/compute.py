#!/usr/bin/python3

import itertools as it
import math
import matplotlib.pyplot as plt

def choose(n, k):
    return math.factorial(n) // (math.factorial(k) * math.factorial(n-k))

def model(pairs):
    perms = math.factorial(2*pairs)
    separated = 0
    for i in range(pairs+1):
        separated += choose(pairs, i) * math.factorial(2*pairs - i) * ((-2)**i)
        #print(i,separated)
    #print(separated/perms)
    return separated, perms

def recursive(pairs):
    pairs -= 1
    perms = math.factorial(2*pairs)*(2*pairs+1)*(2*pairs+2)
    separated = math.factorial(pairs+1) * ((-2)**(pairs+1))
    for i in range(pairs+1):
        #separated += math.factorial(2*pairs - i) * math.factorial(pairs) // (math.factorial(i) * math.factorial(pairs-i)) * ((-2)**i)
        #separated += math.factorial(pairs + i) * math.factorial(pairs) // (math.factorial(i) * math.factorial(pairs-i)) * ((-2)**(pairs-i))
        separated += math.factorial(2*pairs - i) * math.factorial(pairs) // (math.factorial(i) * math.factorial(pairs-i)) * ((-2)**i) * (2*pairs-i+1)*(2*pairs-i+2)*(pairs+1) // (pairs-i+1)
        #print(i,separated)
    print(separated/perms)
    return separated, perms

def has_couple_together(seq):
    for i in range(len(seq)-1):
        if seq[i] ^ 1 == seq[i+1]:
            return True
    return False

def brute_force(pairs):
    perms = 0
    separated = 0
    for p in it.permutations(range(2*pairs)):
        perms += 1
        if not has_couple_together(p):
            separated += 1
    return separated, perms

def check(pairs=5):
    theory = recursive(pairs)
    experiment = brute_force(pairs)
    print(theory, experiment)
    assert theory == experiment

def plot(end, step=1):
    start = 2
    #start = step * int(math.ceil(start/step))
    plt.plot(range(start, end+1, step),
             [s/p for s, p in [model(i) for i in range(start, end+1, step)]])
    plt.ylim(0.330, 0.370)
    plt.xlabel('number of couples')
    plt.ylabel('probability that all couples are separated')
    plt.savefig('riddler-20161118-separated-couples-%d.png' % end)

    plt.show()


def main():
    check(1)
    check(2)
    check(3)
    check(4)
    check(5)

    plot(20)
    plot(50)
    plot(100, step=1)
    plot(1000, step=10)

if __name__ == '__main__':
    main()

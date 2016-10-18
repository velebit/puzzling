#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as mp

SIZE = 20

def all_paths(sum_threshold):
    steps = [0]
    budget = sum_threshold
    while True:
        if steps[-1] < sum_threshold:
            steps.extend(list(range(steps[-1]+1, sum_threshold+1)))
        yield steps[1:]
        while steps[-1] == (steps[-2]+6):
            steps.pop()
            budget += 6
            if len(steps) == 1:
                return
        steps[-1] += 1
        budget -= 1

all_paths_total_probability = 0
triples = np.zeros((SIZE+1,SIZE+1,SIZE+1))

for steps in all_paths(SIZE+1):
    probability = 1/(6**len(steps))
    all_paths_total_probability += probability
    assert steps[-1] >= SIZE+1
    steps = steps[:-1]

    # Add to the probability of all possible triplets where one or
    # more of the numbers is in steps, without double counting.
    # (This could be done with numpy slices &c, but on my HW this
    # iteration seems noticeably more efficient...)
    for i in range(1, SIZE+1):
        i_in_steps = i in steps
        for j in range(i+1, SIZE+1):
            i_or_j_in_steps = i_in_steps or j in steps
            for k in range(j+1, SIZE+1):
                if i_or_j_in_steps or k in steps:
                    triples[i, j, k] += probability

print("Cumulative probability: ", all_paths_total_probability)

while True:
    max_loc = np.unravel_index(np.argmax(triples), triples.shape)
    if triples[max_loc] <= 0:
        break
    print("%-12s = %.9f (spacing>=%d)" %
          (list(max_loc), triples[max_loc],
           min(max_loc[1]-max_loc[0], max_loc[2]-max_loc[1])))
    triples[max_loc] = 0

#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as mp

SIZE = 1000

advance = np.asmatrix(np.zeros((SIZE+1, SIZE+1)))
for i in range(SIZE-5):
    advance[(i+1):(i+7),i] = np.ones((6,1))/6
for i in range(SIZE-5,SIZE+1):
    advance[(i+1):(SIZE+1),i] = np.ones((SIZE-i,1))/6

current = np.asmatrix(np.zeros((SIZE+1,1)))
current[0] = 1.0

all_steps = np.asmatrix(np.zeros((SIZE+1,1)))

for i in range(SIZE):
    current = advance * current
    all_steps = np.hstack((all_steps, current))

cumulative = all_steps.sum(axis=1)

expected = 6/(1+2+3+4+5+6)
eps = 0.005*expected
for i in np.nonzero(np.fabs(cumulative - expected).flat >= eps)[0]:
    if i > 0:
        print("%4d: %9.6f %9.6f" %
              (i, np.asscalar(cumulative[i]),
               np.asscalar((cumulative[i] / expected) - 1)))

mp.plot(cumulative[:21], marker='o')
mp.title('Probability of landing on a given space')
mp.xlabel('space number')
mp.ylabel('probability')
mp.savefig('20161014-classic-space-probability.png')
mp.show()

mp.plot((cumulative / expected) - 1, marker='o')
mp.title('Convergence of landing on a given space')
mp.xlabel('space number')
mp.ylabel('relative difference from probability limit')
mp.xlim((10,50))
mp.ylim((-0.005,0.005))
mp.savefig('20161014-classic-space-probability-convergence.png')
mp.show()

markers=('o', '+', 'x', '*')
for i in range(1, 6*len(markers)+1):
    mp.plot(all_steps[i,:11].T, marker=markers[int((i-1)/6)], label=i)
mp.title('Probability of landing on a space in a particular move')
mp.xlabel('move number')
mp.ylabel('probability')
mp.legend(title='space', ncol=len(markers), fontsize='xx-small')
mp.savefig('20161014-classic-space-move-probability.png')
mp.show()

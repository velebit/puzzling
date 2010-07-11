#!/usr/bin/env python
# ...not fully implemented yet...
from numpy import *
from pylab import *

data = loadtxt("srplot_hist_7_7_2.csv", delimiter=',')

rows = size(data, 0)
for i in range(rows):
    #plot(data[i], '-o')
    bar(array(range(data[i].size))+.8*((i+.1)/rows-.5), data[i], width=.64/rows)

show()

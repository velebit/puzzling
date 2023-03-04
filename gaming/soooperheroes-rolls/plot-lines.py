#!/usr/bin/env python
import numpy
from numpy import *
from pylab import *
from sys import argv
import re
import string

# parse the first command line argument (filename):
csv_file = argv[1]

# optional parameter variables
plot_title = x_label = y_label = x_max = y_max = save_file = save_ext = None
no_save = no_gui = x_lim = y_lim = None
show_legend = None

# defaults
save_ext="png"
markers = [ 'o', 'v', '^', '<', '>',
            #'1', '2', '3', '4',   # hard to see!
            's', 'p', #'*',
            #'h', 'H',             # hard to distinguish from 'o', 'p'
            #'+', 'x',             # hard to see!
            'D', 'd',
            #'|', '_',             # hard to see!
            ]

# load the data:
data = loadtxt(csv_file, delimiter=',', comments='!')

# load the variables:
vars = string.join([line[1:]
                    for line in open(csv_file, 'r').readlines()
                    if re.search(r'^![^!]', line)],
                   "")
exec vars

# parse the rest of the command line, overriding variables from the file:
for i in range(2, size(argv)):
    exec argv[i]

# slice up the data set as desired
if not x_lim is None:
    data = data[:,0:(x_lim+1)]
if not y_lim is None:
    data = data[0:(y_lim+0),:]  # note: rows start at 1

# plot the data:
if data.ndim < 2:
    # if the CSV file has 1 row, we only get a 1D array out of loadtxt.
    plot(data, '-o')
else:
    # plot each row separately, so we can label each line in the legend.
    rows = size(data, 0)
    for i in range(rows):
        plot(data[i], '-', marker=markers[i % size(markers)], label=("%d" % i))
    if not show_legend is None:
        legend()

if not plot_title is None:
    title(plot_title)
if not x_label is None:
    xlabel(x_label)
if not y_label is None:
    ylabel(y_label)
if not x_max is None:
    xlim(xmin=0, xmax=x_max)
if not y_max is None:
    ylim(ymin=0, ymax=y_max)

if save_file is None:
    save_file = csv_file.replace('.csv', '')
if no_save is None:
    savefig(save_file + '.' + save_ext)
if no_gui is None:
    show()

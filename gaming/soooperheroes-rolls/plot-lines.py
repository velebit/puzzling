#!/usr/bin/env python
from numpy import *
from pylab import *
from sys import argv

# parse the command line:
plot_title = x_label = y_label = save_file = ''

csv_file = argv[1]
if size(argv) > 2:
    plot_title = argv[2]
if size(argv) > 3:
    x_label = argv[3]
if size(argv) > 4:
    y_label = argv[4]
if size(argv) > 5:
    save_file = argv[5]

# load the data:
data = loadtxt(csv_file, delimiter=',')

if data.ndim < 2:
    # if the CSV file has 1 row, we only get a 1D array out of loadtxt.
    plot(data, '-o')
else:
    # plot each row separately, so we can label each line in the legend.
    rows = size(data, 0)
    for i in range(rows):
        plot(data[i], '-o', label=("%d" % i))
    #legend()

if len(plot_title) > 0:
    title(plot_title)
if len(x_label) > 0:
    xlabel(x_label)
if len(y_label) > 0:
    ylabel(y_label)

if len(save_file) > 0:
    # if a save file was specified, ONLY save.
    savefig(save_file)
else:
    savefig(csv_file.replace('.csv', '') + '.png')
    show()

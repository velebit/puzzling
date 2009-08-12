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
plot_title = x_label = y_label = x_max = y_max = save_file = None
show_legend = no_gui = None

# defaults
x_label="skill levels"
y_label="dice (CP cost)"
skill_cost=5

# load the data:
data = loadtxt(csv_file, delimiter=',', comments='!')
# ...and add a first row ("0 dice") composed entirely of zeroes:
data = insert(data, 0, 0, axis=0)

# load the variables:
vars = string.join([line[1:]
                    for line in open(csv_file, 'r').readlines()
                    if re.search(r'^![^!]', line)],
                   "")
exec vars

# parse the rest of the command line, overriding variables from the file:
for i in range(2, size(argv)):
    exec argv[i]

# set up the data axis values:
dice      = arange(size(data,0))
skill     = arange(size(data,1))
dice_cost = dice*(dice+1)/2
max_data  = data.max()
z_step    = ceil(max_data/10)
slices    = arange(z_step, max_data * 1.001, z_step)

# plot data and contour labels:
ch = contour(skill, dice_cost, data, slices)
clabel(ch, fmt="%.1f")
# set the ticks/grid lines:
xticks(skill)
dice_labels = ["%dd (%d)" % (d,d*(d+1)/2) for d in dice]
dice_labels[0] = ''
yticks(dice_cost, dice_labels)
grid(True, alpha=0.2)
# set limits to match the last grid line:
xlim(xmin=0, xmax=xticks()[0][-1])
ylim(ymin=0, ymax=yticks()[0][-1])
# set aspect assuming 5 (or whatever) pts/level:
gca().set_aspect(1./skill_cost)

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
    save_file = csv_file.replace('.csv', '') + '.png'
savefig(save_file)
if no_gui is None:
    show()

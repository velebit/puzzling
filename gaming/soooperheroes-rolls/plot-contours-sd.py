#!/usr/bin/env python
import numpy
from numpy import *
from pylab import *
from sys import argv

# parse the command line:
plot_title = save_file = ''

csv_file = argv[1]
if size(argv) > 2:
    plot_title = argv[2]
if size(argv) > 3:
    save_file = argv[3]

# load the data:
data = loadtxt(csv_file, delimiter=',')

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
#yticks(dice_cost, dice)
#xticks(skill, ["%d\n%d" % (s,5*s) for s in skill])
dice_labels = ["%dd (%d)" % (d,d*(d+1)/2) for d in dice]
dice_labels[0] = "0"
yticks(dice_cost, dice_labels)
grid(True, alpha=0.2)
# set limits to match the last grid line:
xlim(xmax=xticks()[0][-1])
ylim(ymax=yticks()[0][-1])
# set aspect assuming 5 pts/level:
gca().set_aspect(1./5)

if len(plot_title) > 0:
    title(plot_title)
xlabel("skill levels")
ylabel("dice (CP cost)")

if len(save_file) > 0:
    # if a save file was specified, ONLY save.
    savefig(save_file)
else:
    savefig(csv_file.replace('.csv', '') + '.png')
    show()

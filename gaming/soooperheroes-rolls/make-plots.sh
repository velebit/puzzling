#!/bin/sh

rm -f *.csv *.png
make soooperheroes_rolls.exe

run() {
    echo "> $*"
    "$@"
}

### contour plots of successes for dice and skill 
./soooperheroes_rolls.exe -pe 10 7 10  -pa 'no_gui=1'
./soooperheroes_rolls.exe -pe 10 9 10  -pa 'no_gui=1'
#./soooperheroes_rolls.exe -pe 10 12 10 -pa 'no_gui=1'

## line plots of the same (reusing the .csv files)
run ./plot-lines.py srplot_expc_f7_d1-10_s0-10.csv \
    "save_file='srplot_expm_f7_d1-10_s0-10.png'" \
    "x_label='skill'" "y_label='average expected successes'" y_max=7 no_gui=1
run ./plot-lines.py srplot_expc_f9_d1-10_s0-10.csv \
    "save_file='srplot_expm_f9_d1-10_s0-10.png'" \
    "x_label='skill'" "y_label='average expected successes'" y_max=7 no_gui=1

### probability plots of successes for skill and various dice and difficulties
./soooperheroes_rolls.exe -psp 3 7 8  -pa 'x_max=8 y_max=60 no_gui=1'
./soooperheroes_rolls.exe -psp 4 7 10 -pa 'x_max=8 y_max=60 no_gui=1'
./soooperheroes_rolls.exe -psp 5 7 10 -pa 'x_max=8 y_max=60 no_gui=1'
./soooperheroes_rolls.exe -psp 6 7 10 -pa 'x_max=8 y_max=60 no_gui=1'
./soooperheroes_rolls.exe -psp 7 7 10 -pa 'x_max=8 y_max=60 no_gui=1'
./soooperheroes_rolls.exe -psp 8 7 10 -pa 'x_max=8 y_max=60 no_gui=1'

./soooperheroes_rolls.exe -psp 7 9 10 -pa 'x_max=8 y_max=60 no_gui=1'
./soooperheroes_rolls.exe -psp 8 9 10 -pa 'x_max=8 y_max=60 no_gui=1'

### plots of expected successes for skill and various dice and difficulties
./soooperheroes_rolls.exe -pse 3 7 10 -pa 'y_max=7 no_gui=1'
./soooperheroes_rolls.exe -pse 4 7 10 -pa 'y_max=7 no_gui=1'
./soooperheroes_rolls.exe -pse 5 7 10 -pa 'y_max=7 no_gui=1'
./soooperheroes_rolls.exe -pse 6 7 10 -pa 'y_max=7 no_gui=1'
./soooperheroes_rolls.exe -pse 7 7 10 -pa 'y_max=7 no_gui=1'
./soooperheroes_rolls.exe -pse 8 7 10 -pa 'y_max=7 no_gui=1'

./soooperheroes_rolls.exe -pse 7 9 10 -pa 'y_max=7 no_gui=1'
./soooperheroes_rolls.exe -pse 8 9 10 -pa 'y_max=7 no_gui=1'

#!/bin/sh

run() {
    echo "| $* &"
    "$@" &
    sleep 1
}

### wipe old files
rm -f *.csv
rm -f *.png *.pdf
make soooperheroes_rolls.exe

### contour plots of successes for dice and skill 
for f in 7 9; do
    ./soooperheroes_rolls.exe -pe 20 $f 50 --no-plot &
done
wait

### zoomed-in (10x10) version
for f in 7 9; do
    for e in png pdf; do
	run ./plot-contours-sd.py srplot_expc_d1-20_s0-50_f$f.csv \
	    "save_ext='$e'" no_gui=1
	run ./plot-contours-sd.py srplot_expc_d1-20_s0-50_f$f.csv \
	    "save_file='srplot_expc_d1-10_s0-10_f$f'" \
	    x_lim=10 y_lim=10 \
	    "plot_title='(1-10 dice, for ${f}s, skill 0-10)'" \
	    "save_ext='$e'" no_gui=1
    done
done

## line plots of the same (reusing the .csv files)
for f in 7 9; do
    run ./plot-lines.py srplot_expc_d1-20_s0-50_f$f.csv \
	"save_file='srplot_expm_d1-20_s0-50_f$f'" \
	"x_label='skill'" "y_label='average expected successes'" \
	y_max=18 no_gui=1
    run ./plot-lines.py srplot_expc_d1-20_s0-50_f$f.csv \
	"save_file='srplot_expm_d1-10_s0-10_f$f'" \
	x_lim=10 y_lim=10 "plot_title='(1-10 dice, for ${f}s, skill 0-10)'" \
	"x_label='skill'" "y_label='average expected successes'" \
	y_max=7 no_gui=1
done

### probability plots of successes for skill and various dice and difficulties
./soooperheroes_rolls.exe -psp 3 7 8  -pa "x_max=8 y_max=60 no_gui=1"
./soooperheroes_rolls.exe -psp 4 7 10 -pa "x_max=8 y_max=60 no_gui=1"
./soooperheroes_rolls.exe -psp 5 7 10 -pa "x_max=8 y_max=60 no_gui=1"
./soooperheroes_rolls.exe -psp 6 7 10 -pa "x_max=8 y_max=60 no_gui=1"
./soooperheroes_rolls.exe -psp 7 7 10 -pa "x_max=8 y_max=60 no_gui=1"
./soooperheroes_rolls.exe -psp 8 7 10 -pa "x_max=8 y_max=60 no_gui=1"

./soooperheroes_rolls.exe -psp 7 9 10 -pa "x_max=8 y_max=60 no_gui=1"
./soooperheroes_rolls.exe -psp 8 9 10 -pa "x_max=8 y_max=60 no_gui=1"

### plots of expected successes for skill and various dice and difficulties
./soooperheroes_rolls.exe -pse 3 7 10 -pa "y_max=7 no_gui=1"
./soooperheroes_rolls.exe -pse 4 7 10 -pa "y_max=7 no_gui=1"
./soooperheroes_rolls.exe -pse 5 7 10 -pa "y_max=7 no_gui=1"
./soooperheroes_rolls.exe -pse 6 7 10 -pa "y_max=7 no_gui=1"
./soooperheroes_rolls.exe -pse 7 7 10 -pa "y_max=7 no_gui=1"
./soooperheroes_rolls.exe -pse 8 7 10 -pa "y_max=7 no_gui=1"

./soooperheroes_rolls.exe -pse 7 9 10 -pa "y_max=7 no_gui=1"
./soooperheroes_rolls.exe -pse 8 9 10 -pa "y_max=7 no_gui=1"

wait

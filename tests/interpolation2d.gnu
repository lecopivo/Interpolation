#!/usr/bin/gnuplot
set grid nopolar
#set style data lines
set grid
splot 'exact2d.dat'
replot 'constant2d.dat'
replot 'linear2d.dat'
replot 'cubic2d.dat'
pause -1

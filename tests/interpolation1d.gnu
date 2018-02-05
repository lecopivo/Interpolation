#!/usr/bin/gnuplot
set style data lines
plot 'exact1d.dat'
replot 'constant1d.dat'
replot 'linear1d.dat'
replot 'cubic1d.dat'
pause -1

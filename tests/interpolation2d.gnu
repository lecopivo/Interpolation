#!/usr/bin/gnuplot
splot 'exact2d.dat' using 1:2:3 with points palette pointsize 2 pointtype 7
replot 'constant2d.dat' using 1:2:3 with points palette pointsize 2 pointtype 7
replot 'linear2d.dat' using 1:2:3 with points palette pointsize 2 pointtype 7
replot 'cubic2d.dat' using 1:2:3 with points palette pointsize 2 pointtype 7
pause -1

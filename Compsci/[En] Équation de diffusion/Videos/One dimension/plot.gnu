reset

set size ratio -1
data = "/Users/antoinehugounet/Documents/Scolarité/UiO/FYS3150 - Computational physics/Project 5/Calypso/Results/Vidéos/One dimension/results"

set title "Diffusion equation in one dimension, explicit scheme, t=0.500000"
set xlabel 'x'
set ylabel 'y'
set xrange [0:1]
set yrange [0:1]
set view map
set cblabel "u(x, y, t)"

splot data using 1:2:3 with image notitle

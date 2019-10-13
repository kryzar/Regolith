reset

set size ratio -1
data = "/Users/antoinehugounet/Documents/Scolarité/UiO/FYS3150 - Computational physics/Project 5/Calypso/Results/Vidéos/Two dimensions/Vidéo 4/results"

set title "Diffusion equation in two dimensions, explicit scheme, t=0.499600"
set xlabel 'x'
set ylabel 'y'
set xrange [0:1]
set yrange [0:1]
set view map
set cblabel "u(x, y, t)"

splot data using 1:2:3 with image notitle

reset
set terminal png enhanced size 1000, 1000
set output '/Users/antoinehugounet/Documents/Scolarité/UiO/FYS3150 - Computational physics/Project 3/Perseids/Program/Results/Peri/Gnuplot/plot-energies.png'
set size ratio -1
set title 'Time-step: 100.000000 years'
set xlabel 'Time (year)'
set ylabel 'Energie (kg*AU^2 / year^2)'

kinetic = '/Users/antoinehugounet/Documents/Scolarité/UiO/FYS3150 - Computational physics/Project 3/Perseids/Program/Results/Peri//system-kinetic-energy
potential = '/Users/antoinehugounet/Documents/Scolarité/UiO/FYS3150 - Computational physics/Project 3/Perseids/Program/Results/Peri//system-potential-energy

total = '/Users/antoinehugounet/Documents/Scolarité/UiO/FYS3150 - Computational physics/Project 3/Perseids/Program/Results/Peri//system-total-energy
plot kinetic using 1:2 w l title 'Kinetic energy', potential using 1:2 w l title 'Potential energy', total using 1:2 w l title 'Total'

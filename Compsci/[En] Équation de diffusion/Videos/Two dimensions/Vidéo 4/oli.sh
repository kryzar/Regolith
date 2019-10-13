#!/bin/bash

i="0.0001"
inc="0.0005"
max="0.5"

for i in $(seq $i $inc $max)
do
    	a=$(echo $i |sed 's/\,/./')
    	echo "Time :" $a
    	/Users/antoinehugounet/Documents/Scolarité/UiO/FYS3150\ -\ Computational\ physics/Project\ 5/Calypso/Results/Vidéos/Two\ dimensions/Vidéo\ 4/Program $a
	    gnuplot -c "/Users/antoinehugounet/Documents/Scolarité/UiO/FYS3150 - Computational physics/Project 5/Calypso/Results/Vidéos/Two dimensions/Vidéo 4/plot-png.gnu"
done

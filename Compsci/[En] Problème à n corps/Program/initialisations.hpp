//
//  initialisations.hpp
//  Program
//
//  Created by Antoine Hugounet on 21/10/2017.
//  Copyright © 2017 Hugounet and Villeneuve. All rights reserved.
//

#pragma once
#include <cmath>
#include "classes/planet.hpp"

//  planets initialisations

//  full solar system, 27.10.2017, 00:00:00
//  data from the NASA, to be obtained at https://ssd.jpl.nasa.gov/horizons.cgi#top

planet earth("earth", 6.E24, 8.30757514E-01, 5.54644964E-01, -9.79193739E-03, 1.42820162E-02);
planet jupiter ("jupiter", 1.9E27, -4.54463137, -2.98088727, 4.05019642E-03, -5.95135698E-03);
planet mars("mars", 6.6E23, -1.60063680E+00, 4.51266379E-01, -3.22884752E-03, -1.22815747E-02);
planet mercury("mercury", 3.3E23, -1.537256803720000E-01, -4.326509049973161E-01, 2.084982792680136E-02, -8.026395475494962E-03);
planet neptune("neptune", 1.03E26, 2.862286355822386E+01, -8.791151564880529E+00, 9.010839253968958E-04, 3.019851091079401E-03);
planet saturn("saturn", 5.5E26, -3.000365464147523E-01, -1.005121487413227E+01, 5.269787506575000E-03, -1.837774500581324E-04);
planet sun("sun", 2.E30, 2.17112305E-03, 5.78452455E-03, -5.30635989E-06, 5.44444408E-06);
planet uranus("uranus", 8.8E25, 1.784192250600989E+01, 8.843248837812927E+00, -1.775564692276439E-03, 3.340680253439256E-03);
planet venus("venus", 4.9E24, -7.049960476048377E-01, 1.312910376443176E-01, -3.646727612165235E-03, -1.999997620788355E-02);


//  data calculated manually for the Earth-Jupiter-Sun system
//  sun as the mass center, wmc = wrong mass center

planet earth_ejs_wmc("earth", 6.E24, 1., 0., 0., 2*M_PI/365.25);
planet jupiter_ejs_wmc("jupiter", 1.9E27, 5.20, 0., 0., (2*M_PI*5.20)/(11.862*365.25));
planet sun_wmc("sun", 2.E30, 0, 0, 0, 0);

//  real mass center of the new system, rm = real mass center

planet earth_ejs_rmc("earth", 6.E24, 0.995064, 0., 0., 6.25217 / 365.25);
planet jupiter_ejs_rmc("jupiter", 1.9E27, 5.19506, 0., 0., 2.75177 / 365.25);
planet sun_ejs_rmc("sun", 2.E30, -4.936E-3, 0., 0., -2.63294E-3 / 365.25);

//  perihelion of Mercury

planet mercury_peri("mercury", 3.3E23, 0.3075, 0., 0., (12.44 / 365.25));


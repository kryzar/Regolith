//
//  main.cpp
//  Program
//
//  Created by Antoine Hugounet on 08/10/2017.
//  Copyright © 2017 Hugounet and Villeneuve. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "unit-tests.hpp"
#include "initialisations.hpp"
#include "classes/planet.hpp"
#include "classes/solver.hpp"
#include "time.h"
#include <cmath>

using namespace std;


int main(int argc, const char* argv[])
{
    
    if(run_unittest(0, nullptr) != 0)
    {
        cout << "Unittests failed." << endl;
        exit(1);
    }

    string folder = "/Users/antoinehugounet/Documents/Scolarité/UiO/FYS3150 - Computational physics/Project 3/Perseids/Program/Results/Tests/";
    
    solver system;
    system.add(sun);
    system.add(earth);
    system.add(mars);
    system.add(jupiter);
    system.add(uranus);
    system.add(neptune);
    system.add(mercury);
    system.add(saturn);
    system.add(venus);
    
    clock_t start;
    clock_t finish;
    
    start = clock();
    system.verlet(100., folder);
    finish = clock();
    
    cout << ((double) finish - (double) start) / CLOCKS_PER_SEC;
    
    return 0;
}


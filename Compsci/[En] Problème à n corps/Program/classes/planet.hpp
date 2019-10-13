//
//  planet.hpp
//  Program
//
//  Created by Antoine Hugounet on 08/10/2017.
//  Copyright © 2017 Hugounet and Villeneuve. All rights reserved.
//


#pragma once
#include <vector>
#include <string>
#include <fstream>


class planet
{

public:

    //  constructors

    planet(void);
    planet(std::string name, double mass, double x, double y, double vx, double vy);
    planet(const planet& body);

    //  data

    double time;   //  in years
    std::vector<double> position;
    std::vector<double> velocity;

    //  getters

    int dim(void) const;  //  dimension of the vectors (2 or 3)
    double mass(void) const;
    std::string name(void) const;

    //  methods

    double distance(const planet& body) const;  //  distance to the other planet
    double distance_center(void) const; //  distance to the origin
    double kinetic_energy(void) const;
    double potential_energy(const planet& body) const;
    double potential_energy(const std::vector<planet>& system) const;
    double total_energy(const planet& body) const;
    double total_energy(const std::vector<planet>& system) const;
    double velocity_squared(void) const;    //  square of the velocity norm
    void normalize(void);   //  normalize with the good time & distance units
    void print(std::ofstream& output) const;  //  outputs position and velocity
    void print(std::ofstream& output, const std::vector<planet>& system) const;  //  idem + energies
    void print_pos(std::ofstream& output) const;  //  used by the solver class
    void print_vel(std::ofstream& output) const;  //  idem


private:

    //  data

    int _dim;
    double _mass;
    std::string _name;
};

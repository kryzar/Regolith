//
//  planet.cpp
//  Program
//
//  Created by Antoine Hugounet on 08/10/2017.
//  Copyright © 2017 Hugounet and Villeneuve. All rights reserved.
//


#include <iostream>
#include <iomanip>
#include "planet.hpp"
#include <cmath>
#include <vector>
#include <string>

using namespace std;

//  constructors

planet::planet(void)
{

    time = 0;
    _name = "AUTO NAMING";
    _dim = 2;
    _mass = 1.;

    position = {1., 0.};
    velocity = {0., 0.};
}

////////

planet::planet(std::string name, double mass, double x, double y, double vx, double vy)
{
    time = 0.;
    _name = name;
    _dim = 2;
    _mass = (double) mass;

    position = {(double) x, (double) y};
    velocity = {(double) vx, (double) vy};

    time = 0;
}

////////

planet::planet(const planet& body)
{
    time = (double) body.time;
    _name = body._name;
    _dim = (double) body._dim;
    _mass = (double) body._mass;

    position = body.position;
    velocity = body.velocity;

}

//  getters

int planet::dim(void) const
{
    return (_dim);
}

////////

double planet::mass(void) const
{
    return (_mass);
}

////////

std::string planet::name(void) const
{
    return (_name);
}

//  methods

double planet::distance(const planet& body) const
{
    double sum = 0.;
    double relative_position;

    for(int i = 0; i < _dim; i++)
    {
        relative_position = position[i] - body.position[i];
        sum += relative_position * relative_position;
    }

    return (sqrt(sum));
}

////////

double planet::distance_center(void) const
{
    double sum = 0.;

    for(int i = 0; i < _dim; i++)
    {
        sum += position[i] * position[i];
    }

    return (sqrt(sum));
}

////////

double planet::kinetic_energy(void) const
{
    double energy = 0.5 * _mass * velocity_squared();
    
    return (energy);
}

////////

//  the potential energy is with respect to another body
double planet::potential_energy(const planet& body) const
{
    double energy;
    
    if(distance(body) != 0.)
    {
        double r = distance(body);
        
        energy = (4 * M_PI * M_PI * _mass * body._mass) / (r*r);
    }
    else
    {
        energy = 0.;
    }
    
    return (energy);
}

////////

//  we can calculate it for many planets
//  this one is usefull for solver::potential_energy
double planet::potential_energy(const std::vector<planet>& system) const
{
    double energy = 0.;

    for(auto& body : system)
    {
        energy -= potential_energy(body);
    }

    return (energy);
}

////////

double planet::total_energy(const planet& body) const
{
    double energy = kinetic_energy() + potential_energy(body);
    
    return (energy);
}

////////

double planet::total_energy(const std::vector<planet>& system) const
{
    double energy = kinetic_energy() + potential_energy(system);

    return (energy);
}

////////

double planet::velocity_squared(void) const
{
    double v = 0.;
    
    for(int i = 0; i < _dim; i++)
    {
        v += velocity[i] * velocity[i];
    }
    
    return (v);
}

////////

void planet::normalize(void)
{
    //  the program is made to work with NASA's data
    //  the velocities are given in AU/day
    _mass /= 2.E30;
    velocity[0] *= 365.25;
    velocity[1] *= 365.25;
}

////////

void planet::print(std::ofstream& output) const
{
    output << _name << endl;
    output << _mass << "kg" << endl;
    output << "At t=" << time << " years" << endl;

    output << "Position: " << endl;
    for(int i = 0; i < _dim; i++)
    {
        output << setprecision(12) << position[i] << endl;
    }
    output << "Velocity: " << endl;
    for(int i = 0; i < _dim; i++)
    {
        output << setprecision(12) << velocity[i] << endl;
    }

    output << endl;

}

////////

void planet::print(std::ofstream& output, const std::vector<planet>& system) const
{
    output << _name << endl;
    output << _mass << "kg" << endl;
    output << "At t=" << time << " years" << endl;
    output << "Kinetic energy: " << kinetic_energy() << endl;
    output << "Potential energy: " << potential_energy(system) << endl;
    output << "Total energy: " << total_energy(system) << endl;

    output << "Position: " << endl;
    for(int i = 0; i < _dim; i++)
    {
        output << setprecision(12) << position[i] << endl;
    }
    output << "Velocity: " << endl;
    for(int i = 0; i < _dim; i++)
    {
        output << setprecision(12) << velocity[i] << endl;
    }

    output << endl;

}

////////

//  those ones are useful for Verlet and Euler
//  they are made to have clean columns in output files

void planet::print_pos(std::ofstream& output) const
{
    string space = "        ";

    for(int i = 0; i < _dim; i++)
    {
        output << setprecision(12) << position[i] << space;
    }
}

////////

void planet::print_vel(std::ofstream& output) const
{
    string space = "        ";

    for(int i = 0; i < _dim; i++)
    {
        output << setprecision(12) << velocity[i] << space;
    }
}

//
//  solver.hpp
//  Program
//
//  Created by Antoine Hugounet on 08/10/2017.
//  Copyright © 2017 Hugounet and Villeneuve. All rights reserved.
//


#pragma once
#include <vector>
#include "planet.hpp"
#include <fstream>
#include <cmath>



class solver
{
    friend class planet;
    
public:
    
    //  constructors
    
    solver(void);
    solver(const solver& other);
    
    //  main algorithms
    
    void euler(const double years, const std::string folder);
    void verlet(const double years, const std::string folder, const bool relativity = false, const bool highres = false);
    
    //  getters
    
    int size(void) const;
    double time(void) const;
    double total_mass(void) const;
    
    //  methods

    double kinetic_energy(void) const;
    double potential_energy(void) const;
    double total_energy(void) const;
    //  if you will calculate Verlet with a relativistic corection, you must specify it now
    void add(planet body, const bool relativity = false);
    void print(std::ofstream& file) const;  //  prints the system's last position and velocity
    std::vector<double> mass_center(void) const;
    std::vector<planet> system(void) const;

    
private:

    //  data
    
    int _card;  //  number of planets in the system
    double _time;  //  t_0 of the system, in years
    double _total_mass;
    std::vector<double> _mass_center;
    std::vector<std::vector<double>> _prev_pos; //  contains all the position at ti
    std::vector<std::vector<double>> _prev_vel;
    std::vector<std::vector<double>> _prev_acc;
    std::vector<std::vector<double>> _next_acc;
    std::vector<planet> _system;    //  contains all the planet
    
    //  methods
    
    void _update_mass_center(const planet& body);
    void _update_quantities(const int i, const double h);   //  update quantities at each lop
    void _update_quantities(const int i, const double h, std::vector<std::vector<double>> acc);
    std::vector<double> _acceleration(const int p, const bool relativity = false) const;    //  p is the index of the planet in _system
    std::vector<std::vector<double>> _next_acceleration(const bool relativity) const;
    
    //  outputs
    inline void _classic_output(const bool can_write, const int k, const int i, const std::string folder) const;
    void _first_output(const int k, const int i, const double years, const std::string folder) const;
    void _perihelion_output(const bool relativity, const bool highres, const int k, const int i, const double years, const std::string folder) const;
    void _print_kinetic_energy(const int i, const std::string folder) const;
    void _print_potential_energy(const int i, const std::string folder) const;
    void _print_total_energy(const int i, const std::string folder) const;
    void _gnuplot(const std::string folder, const double years) const;
    void _gnuplot_png(const std::string folder, const double years) const;
    void _gnuplot_energies(const std::string folder, const double years) const;
    void _gnuplot_energies_png(const std::string folder, const double years) const;
    std::string _gnuplot_colors(const int k) const;

};


inline void solver::_classic_output(const bool can_write, const int k, const int i, const std::string folder) const
{
    std::string path;
    std::ofstream output;
    
    if(can_write && i != 0)
    {
        path = folder + _system[k].name();
        output.open(path, std::ios::app);
        _system[k].print_pos(output);
        _system[k].print_vel(output);
        output << std::endl;
        output.close();
    }
}

inline void solver::_first_output(const int k, const int i, const double years, const std::string folder) const
{
    std::string path;
    std::ofstream output;
    
    if(i == 0)
    {
        path = folder + _system[k].name();
        output.open(path);  //  erase the previous file
        output << "Velocity-Verlet algorithm (2D)" << std::endl;
        output << _system[k].name() << " (x, y, vx, vy)" << std::endl;
        output << "Timestep: " << years << " years" << std::endl << std::endl ;
        if(_system[k].distance_center() == 0)
        {
            //  if the body is the mass center, its values never change
            //  so we print the initial values once, and never compute new ones
            _system[k].print_pos(output);
            _system[k].print_vel(output);
        }
        output.close();
    }
}

inline void solver::_perihelion_output(const bool relativity, const bool highres, const int k, const int i, const double years, const std::string folder) const
{
    std::string path;
    std::ofstream output;
    
    if((relativity || highres) && _system[k].name() == "mercury")
    {
        if(i == 0)
        {
            path = folder + "mercury perihelion precession";
            output.open(path);  //  erase the previous file
            output << "Perihelion precession of Mercury (xp, yp, thetap)" << std::endl;
            output << "Timestep: " << years << " earth-years" << std::endl;
            output << "Relativistic correction: " << std::boolalpha << relativity << std::endl;
            output << "High-resolution: " << std::boolalpha << highres << std::endl << std::endl;
            output << _time << "        ";
            _system[k].print_pos(output);
            output << "        " << atan(_system[k].position[1] / _system[k].position[0]);
            output << std::endl;
            output.close();
        }
        
        if(_system[k].distance_center() <= 0.3075 && i != 0)
        {
            path = folder + "mercury perihelion precession";
            output.open(path, std::ios::app);  //  erase the previous file
            output << _time << "        ";
            _system[k].print_pos(output);
            output << "        " << 648000 * atan(_system[k].position[1] / _system[k].position[0]);
            output << std::endl;
            output.close();
        }
    }
}

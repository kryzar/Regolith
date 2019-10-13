//
//  solver-private.cpp
//  Program
//
//  Created by Antoine Hugounet on 8/10/2017.
//  Copyright © 2017 Hugounet and Villeneuve. All rights reserved.
//



#include "solver.hpp"
#include "planet.hpp"
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;


//  constructors


solver::solver(void)
{
    _card = 0;
    _time = 0;
    _total_mass = 0.;
    _mass_center = {0., 0.};
    
}

////////

solver::solver(const solver& other)
{
    
    _card = other._card;
    _time = other._time;
    _total_mass = other._total_mass;
    _mass_center = other._mass_center;
    _system = other._system;
    
    _prev_pos = other._prev_pos;
    _prev_vel = other._prev_vel;
    _prev_acc = other._prev_acc;
    _next_acc = other._prev_acc;
}


//  main algorithms


void solver::euler(const double years, const std::string folder)
{
    //  equations in 1D :
    //  x(t+dt) = dt*v(t) + x(t)
    //  v(t+dt) = dt*a(t) + v(t) = dt*Fx/m + v(t), with Newton's law
    
    int timesteps;
    double h;
    string path;
    ofstream output;
    
    timesteps = (int) (years * 250);
    h = ((double) years) / ((double) timesteps);
    
    //  go through every time-step, then every planet
    for(int i = 0; i <= timesteps; i++)
    {
        for(int k = 0; k < _card; k++)
        {
            path = folder + _system[k].name();
            
            if(i == 0)
            {
                //  only write the header once
                output.open(path);
                output << "Euler algorithm (2D)" << endl;
                output << _system[k].name() << " (x, y, vx, vy)" << endl;
                output << "Timestep: " << years << " years" << endl << endl;
            }
            else
            {
                output.open(path, ios::app);   //  write after the existing content
            }
            
            _system[k].print_pos(output);   //  prints quantities for a gnuplot
            _system[k].print_vel(output);
            output << endl;
            
            //  then perform the algorithm
            //  _prev_pos and vel are initialized when a planet is added
            //  see solver::add
            _system[k].position[0] = h * _prev_vel[k][0] + _prev_pos[k][0];
            _system[k].position[1] = h * _prev_vel[k][1] + _prev_pos[k][1];
            
            _system[k].velocity[0] = h * _prev_acc[k][0] + _prev_vel[k][0];
            _system[k].velocity[1] = h * _prev_acc[k][1] + _prev_vel[k][1];
            
            output.close();
        }
        
        _print_kinetic_energy(i, folder);
        _print_potential_energy(i, folder);
        _print_total_energy(i, folder);
        _update_quantities(i, h);   //  update the _prev_vectors
    }
    
    //  create gnuplot scripts
    _gnuplot(folder, years);
    _gnuplot_png(folder, years);
    _gnuplot_energies(folder, years);
    _gnuplot_energies_png(folder, years);
    
    _time += years;
}

////////

void solver::verlet(const double years, const std::string folder, const bool relativity, const bool highres)
{
    //  equations in 1D :
    //  x(t+dt) = x(t) + dt*v(t) + (1/2)(dt^2)*a(t)
    //  v(t+dt) = v(t) + (1/2)*dt*[a(t) + a(t+dt)]
    //  the relativity boolean tells the program to compute or not the relativistic correction
    //  works only for the Mercury-Sun system for the moment
    
    int timesteps;
    double h;
    double h_squared;
    double radical;
    bool can_write;
    bool mass_center;
    string path;
    ofstream output;
    vector<vector<double>> next_acc;
    
    if(relativity && !highres)
    {
        cout << "You can't compute the relativity without a high-res." << endl;
        exit (1);
    }
    
    timesteps = (relativity || highres) ? ((int) years * 9072000) : ((int) years * 365);
    h = ((double) years) / ((double) timesteps);
    h_squared = h * h;
    
    for(int i = 0; i <= timesteps; i++)
    {
        //  with the relativity or the highres added
        //  the program takes a too long time to write in documents
        //  as a consequence, it outputs only a few values
        can_write = (relativity || highres) ? (i % 24800 == 0) : true;
        
        for(int k = 0; k < _card; k++)
        {
            mass_center = _system[k].distance_center() == 0;
            _perihelion_output(relativity, highres, k, i, years, folder);
            _first_output(k, i, years, folder);
            
            if(!mass_center)
            {
                _classic_output(can_write, k, i, folder);
                
                radical = 0.5 * h_squared;
                _system[k].position[0] = _prev_pos[k][0] + h * _prev_vel[k][0] + radical * _prev_acc[k][0];
                _system[k].position[1] = _prev_pos[k][1] + h * _prev_vel[k][1] + radical * _prev_acc[k][1];
            }
        }
        
        //  computes the new acceleration with the just calculated position
        //  note that the first initialization of _prev_acc can be done with relativity, see solver::add
        _next_acc = _next_acceleration(relativity);
        
        for(int k = 0; k < _card; k++)
        {
            mass_center = _system[k].distance_center() == 0;
            if(!mass_center)
            {
                radical = 0.5 * h;
                _system[k].velocity[0] = _prev_vel[k][0] + radical * (_prev_acc[k][0] + _next_acc[k][0]);
                _system[k].velocity[1] = _prev_vel[k][1] + radical * (_prev_acc[k][1] + _next_acc[k][1]);
            }
        }
        
        //  we don't print the energies for the relativistic case
        //  they indeed would need a correction too
        if(can_write && !relativity)
        {
            _print_kinetic_energy(i, folder);
            _print_potential_energy(i, folder);
            _print_total_energy(i, folder);
        }
        
        //  update of the _prev_vectors
        _update_quantities(i, h, _next_acc);

    }
    
    _gnuplot(folder, years);
    _gnuplot_png(folder, years);
    if(!relativity)
    {
        _gnuplot_energies(folder, years);
        _gnuplot_energies_png(folder, years);
    }
    
    _time += years;
}


//  getters


int solver::size(void) const
{
    return (_card);
}

////////

double solver::total_mass(void) const
{
    return (_total_mass);
}

////////

double solver::time(void) const
{
    return (_time);
}


//  methods


double solver::kinetic_energy(void) const
{
    double energy = 0.;
    
    for(auto& body : _system)
    {
        energy += body.kinetic_energy();
    }
    
    return (energy);
}

////////

double solver::potential_energy(void) const
{
    double energy = 0.;
    
    for(auto& body : _system)
    {
        energy += body.potential_energy(_system);   //  this method is in planet
    }
    
    return (energy);
}

////////

double solver::total_energy(void) const
{
    double energy = kinetic_energy() + potential_energy();
    
    return (energy);
}

////////

void solver::add(planet body, const bool relativity)
{
    _card++;
    
    _update_mass_center(body);
    _total_mass += body.mass();
    
    //  normalize the mass and the velocity
    body.normalize();
    _system.push_back(body);
    _prev_pos.push_back(body.position);
    _prev_vel.push_back(body.velocity);
    _prev_acc.push_back(_acceleration(_card - 1, relativity));
    //  the new planet is the (_card - 1) celestial body of the system
}

////////

//  prints the last positions, velocities
void solver::print(ofstream& file) const
{
    file << "=== CELESTIAL SYSTEM === " << endl;
    
    for(int p = 0; p < _card ; p++)
    {
        _system[p].print(file); //  planet:: method
    }
    
    file << "===/ CELESTIAL SYSTEM === " << endl;
}

////////

vector<double> solver::mass_center(void) const
{
    return (_mass_center);
}

std::vector<planet> solver::system(void) const
{
    return (_system);
}


///////

//  some gnuplot scripts

void solver::_gnuplot(const std::string folder, const double years) const
{
    ofstream outplot;
    string output_path;
    string style;
    string title;
    vector<string> path(_card);    //  path to each data file
    
    output_path = folder + "Gnuplot/plot.gnu";
    outplot.open(output_path);
    
    style = " using 1:2 w l lt rgb ";
    
    outplot << "reset" << endl;
    outplot << "set size ratio -1" << endl;
    outplot << "set title 'Time-step: " << to_string(years) << " years'" << endl;
    outplot << "set xlabel 'distance (AU)'" << endl;
    outplot << "set ylabel 'distance (AU)'" << endl << endl;
    
    for(int k = 0; k < _card; k++)
    {
        path[k] = "'" + folder + _system[k].name() + "'";
        title = " title '" + _system[k].name() + "'";
        
        if(k == 0)
        {
            outplot << "plot " + path[k] + style + _gnuplot_colors(k) + title << endl << endl;
        }
        else
        {
            outplot << "replot " + path[k] + style + _gnuplot_colors(k) + title << endl << endl;
        }
    }
    
    outplot.close();
}

////////

void solver::_gnuplot_png(const std::string folder, const double years) const
{
    ofstream outplot;
    string output_path;
    string style;
    string title;
    vector<string> path(_card);    //  path to each data file
    
    output_path = folder + "Gnuplot/plot-png.gnu";
    outplot.open(output_path);
    
    style = " using 1:2 w l lt rgb ";
    
    outplot << "reset" << endl;
    outplot << "set terminal png enhanced size 1000, 1000" << endl;
    outplot << "set output " << "'" << folder << "Gnuplot/plot.png'" << endl;
    outplot << "set size ratio -1" << endl;
    outplot << "set title 'Time-step: " << to_string(years) << " years'" << endl;
    outplot << "set xlabel 'distance (AU)'" << endl;
    outplot << "set ylabel 'distance (AU)'" << endl << endl;
    
    for(int k = 0; k < _card; k++)
    {
        path[k] = "'" + folder + _system[k].name() + "'";
        title = " title '" + _system[k].name() + "'";
        
        if(k == 0)
        {
            // plot earth using 1:2 w l lt rgb "blue" title 'Earth'
            outplot << "plot " + path[k] + style + _gnuplot_colors(k) + title;
        }
        else
        {
            outplot << ", " + path[k] + style + _gnuplot_colors(k) + title;
        }
    }
    
    outplot.close();
}

////////

void solver::_gnuplot_energies(const std::string folder, const double years) const
{
    ofstream outplot;
    string output_path;
    string style;
    string title;
    vector<string> path(_card);    //  path to each data file
    
    output_path = folder + "Gnuplot/plot-energies.gnu";
    outplot.open(output_path);
    
    outplot << "reset" << endl;
    outplot << "set size ratio -1" << endl;
    outplot << "set title 'Time-step: " << to_string(years) << " years'" << endl;
    outplot << "set xlabel 'Time (year)'" << endl;
    outplot << "set ylabel 'Energie (kg*AU^2 / year^2)'" << endl << endl;
    outplot << "kinetic = '" << folder << "/system-kinetic-energy" << endl;
    outplot << "potential = '" << folder << "/system-potential-energy" << endl << endl;
    outplot << "total = '" << folder << "/system-total-energy" << endl;
    outplot << "plot kinetic using 1:2 w l title 'Kinetic energy'" << endl;
    outplot << "replot potential using 1:2 w l title 'Potential energy'" << endl;
    outplot << "replot total using 1:2 w l title 'Total'" << endl;
    
    outplot.close();
}

////////

void solver::_gnuplot_energies_png(const std::string folder, const double years) const
{
    ofstream outplot;
    string output_path;
    string style;
    string title;
    vector<string> path(_card);    //  path to each data file
    
    output_path = folder + "Gnuplot/plot-energies-png.gnu";
    outplot.open(output_path);
    
    outplot << "reset" << endl;
    outplot << "set terminal png enhanced size 1000, 1000" << endl;
    outplot << "set output " << "'" << folder << "Gnuplot/plot-energies.png'" << endl;
    outplot << "set size ratio -1" << endl;
    outplot << "set title 'Time-step: " << to_string(years) << " years'" << endl;
    outplot << "set xlabel 'Time (year)'" << endl;
    outplot << "set ylabel 'Energie (kg*AU^2 / year^2)'" << endl << endl;
    outplot << "kinetic = '" << folder << "/system-kinetic-energy" << endl;
    outplot << "potential = '" << folder << "/system-potential-energy" << endl << endl;
    outplot << "total = '" << folder << "/system-total-energy" << endl;
    outplot << "plot kinetic using 1:2 w l title 'Kinetic energy', potential using 1:2 w l title 'Potential energy', total using 1:2 w l title 'Total'" << endl;
    
    outplot.close();
}


////////

void solver::_print_kinetic_energy(const int i, const std::string folder) const
{
    ofstream output;
    string space = "        ";
    
    if(i == 0)
    {
        output.open(folder + "system-kinetic-energy");
    }
    else
    {
        output.open(folder + "system-kinetic-energy", ios::app);
    }
    output << _time << setprecision(12) << space << kinetic_energy() << endl;
    
    output.close();
}

////////

void solver::_print_potential_energy(const int i, const std::string folder) const
{
    ofstream output;
    string space = "        ";
    
    if(i == 0)
    {
        output.open(folder + "system-potential-energy");
    }
    else
    {
        output.open(folder + "system-potential-energy", ios::app);
    }
    output << _time << setprecision(12) << space << potential_energy() << endl;
    
    output.close();
}


////////

void solver::_print_total_energy(const int i, const std::string folder) const
{
    ofstream output;
    string space = "        ";
    
    if(i == 0)
    {
        output.open(folder + "system-total-energy");
    }
    else
    {
        output.open(folder + "system-total-energy", ios::app);
    }
    output << _time << setprecision(12) << space << total_energy() << endl;
    
    output.close();
}

////////

void solver::_update_mass_center(const planet& body)
{
    double total_mass;
    
    _mass_center[0] *= _total_mass;
    _mass_center[1] *= _total_mass;
    
    total_mass = _total_mass + body.mass();
    
    _mass_center[0] += body.mass() * body.position[0];
    _mass_center[1] += body.mass() * body.position[1];
    _mass_center[0] /= total_mass;
    _mass_center[1] /= total_mass;
}

////////

//  this one is for Euler, where we don't need the next acceleration
void solver::_update_quantities(const int i, const double h)
{
    for(int k = 0; k < _card; k++)
    {
        _prev_pos[k] = _system[k].position;
        _prev_vel[k] = _system[k].velocity;
        _prev_acc[k] = _acceleration(k, false);
        _system[k].time = i * h;
    }
    _time = i * h;
}

////////

//  this one is for Verlet
void solver::_update_quantities(const int i, const double h, std::vector<std::vector<double>> acc)
{
    for(int k = 0; k < _card; k++)
    {
        _prev_pos[k] = _system[k].position;
        _prev_vel[k] = _system[k].velocity;
        _system[k].time = i * h;
    }
    
    //  it has been calculated once in Verlet, so we avoid a calculation
    _prev_acc = acc;
    _time = i * h;
}


////////

//  only one method for a relativistic or non-relativistic simulation
vector<double> solver::_acceleration(const int p, const bool relativity) const
{
    //  p is the index of the planet for which we calculate eta
    
    double const g_const = 4 * M_PI * M_PI;
    double radical; //  just a variable to avoid calculations
    double r;
    vector<double> relative_pos = {0., 0.};
    vector<double> acceleration = {0., 0.};
    
    if(_system[p].distance_center() != 0.)  // the mass center must remain fixed
    {
        for(int k = 0; k < _card; k++)
        {
            if(k != p)
            {
                r = _system[p].distance(_system[k]);
                double r_squared = r * r;
                double r_cubed = r_squared * r;
                radical = _system[k].mass() / r_cubed;
                
                relative_pos[0] = _system[p].position[0] - _system[k].position[0];  //  x - xk
                relative_pos[1] = _system[p].position[1] - _system[k].position[1];
                
                acceleration[0] -= radical * relative_pos[0];
                acceleration[1] -= radical * relative_pos[1];
                if(relativity)
                {
                    double correction;
                    double momentum;
                    double const c = 63241.0770;
                    momentum = _system[p].position[0] * _system[p].velocity[1] - _system[p].position[1] * _system[p].velocity[0];
                    correction = 1. + (3. * momentum * momentum) / (r_squared * c * c);
                    acceleration[0] *= correction;
                    acceleration[1] *= correction;
                }
            }
        }
        acceleration[0] *= g_const ;
        acceleration[1] *= g_const;
    }
    //  this concerns especially the sun when it is center of mass
    //  this assures that it remains fix if the initial velocity is (0, 0)
    else
    {
        acceleration[0] = 0.;
        acceleration[1] = 0.;
    }
    
    return (acceleration);
}

////////

//  it is just used in verlet to compute a(t+dt) when we want v(t)
std::vector<std::vector<double>> solver::_next_acceleration(const bool relativity) const
{
    vector<vector<double>> acceleration;
    
    for(int k = 0; k < _card; k++)
    {
        acceleration.push_back(_acceleration(k, relativity));
    }
    
    return (acceleration);
}

////////

std::string solver::_gnuplot_colors(const int k) const
{
    string color;
    
    if(_system[k].name() == "earth") color = "\"blue\"";
    else if(_system[k].name() == "jupiter") color = "\"light-goldenrod\"";
    else if(_system[k].name() == "mercury") color = "\"orange-red\"";
    else if(_system[k].name() == "mars") color = "\"brown\"";
    else if(_system[k].name() == "neptune") color = "\"royalblue\"";
    else if(_system[k].name() == "saturn") color = "\"goldenrod\"";
    else if(_system[k].name() == "sun") color = "\"black\"";
    else if(_system[k].name() == "uranus") color = "\"light-blue\"";
    else if(_system[k].name() == "venus") color = "\"dark-goldenrod\"";
    else color = "dark-violet";
    
    return (color);
}

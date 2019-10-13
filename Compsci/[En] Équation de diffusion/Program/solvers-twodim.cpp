//
//  solvers-twodim.cpp
//  Program
//
//  Created by Antoine Hugounet on 24/11/2017.
//

#include "solvers.hpp"
#include <vector>
#include <string>
#include "utilities.hpp"

using namespace std;


void twodim_explicit(const unsigned meshpoints, const double time_final, const unsigned time_steps, const std::string folder)
{
    
    /*
     We want to solve the 2D diffusion equation.
     By scalling and discretizing we come up with a linear algebra system.
     It can be solves with a simple loop, as it is shown here.
    */
    
    const double h = 1. / (double) meshpoints;          //  space-step for both x and y
    const double dt = time_final / (double) time_steps;
    const double alpha = dt / (h * h);
    double delta_xy;
    vector<vector<double>> u(meshpoints + 1, vector<double>(meshpoints + 1));

    alpha_warning(alpha, 0.25);   //  we require alpha > 0.25
    initial_conditions(u, meshpoints);   //  arbitrary boundary conditions, to be modified in utilities.hpp directly
    
    for(double time = 0.; time <= time_final; time += dt)
    {
        for(unsigned x = 1; x < meshpoints; x++)
        {
            for(unsigned y = 1; y < meshpoints; y++)
            {
                delta_xy = u[x+1][y] + u[x-1][y] + u[x][y-1] + u[x][y+1];
                u[x][y]  = u[x][y] + alpha * (delta_xy - 4. * u[x][y]);
            }
        }
    }
    
    //  some outputs and gnuplot scripts
    output(folder, u, time_final);
    gnuplot_twodim(folder, "explicit scheme", time_final);
    gnuplot_twodim_png(folder, "explicit scheme", time_final);
}

void twodim_implicit(const unsigned meshpoints, const double time_final, const unsigned time_steps, const std::string folder)
{
    
    /*
     We want to solve the 2D diffusion equation.
     By scalling and discretizing we come up with a linear algebra system.
     It can be solves with a simple loop, as it is shown here.
    */
    
    const double h = 1. / (double) meshpoints;          //  space-step for both x and y
    const double dt = time_final / (double) time_steps;
    const double alpha = dt / (h * h);
    const double beta = 1. + 4. * alpha;
    double delta_xy;
    vector<vector<double>> u(meshpoints + 1, vector<double>(meshpoints + 1));
    
    initial_conditions(u, meshpoints);   //  arbitrary boundary conditions, to be modified in utilities.hpp directly
    
    for(double time = 0.; time <= time_final; time += dt)
    {
        for(unsigned x = 1; x < meshpoints; x++)
        {
            for(unsigned y = 1; y < meshpoints; y++)
            {
                delta_xy = u[x+1][y] + u[x-1][y] + u[x][y-1] + u[x][y+1];
                u[x][y]  = (alpha * delta_xy + u[x][y]) / beta;
            }
        }
    }
    
    //  some outputs and gnuplot scripts
    output(folder, u, time_final);
    gnuplot_twodim(folder, "implicit scheme", time_final);
    gnuplot_twodim_png(folder, "implicit scheme", time_final);
}

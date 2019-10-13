//
//  solvers-onedim.cpp
//  Program
//
//  Created by Antoine Hugounet on 24/11/2017.
//

#include "solvers.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "utilities.hpp"
#include <math.h>

using namespace std;


void onedim_explicit(const unsigned meshpoints, const double time_final, const unsigned time_steps, const std::string folder)
{
    /*
     We want to solve the 1D diffusion equation.
     By scalling and discretizing we come up with a linear algebra system.
     It can be solves with a simple loop, as it is shown here.
    */
    
    const double h = 1. / (double) meshpoints;          //  space-step
    const double dt = time_final / (double) time_steps;
    const double alpha = dt / (h * h);
    const double beta = 1. - 2. * alpha;
    vector<double> u(meshpoints + 1);                   //  solution vector
    
    alpha_warning(alpha, 0.5);  //  we require alpha > 0.5
    initial_conditions(u);
    
    for(double t = 0.; t <= time_final; t += dt)
    {
        for(unsigned i = 1; i < meshpoints; i++)
        {
            u[i] = alpha * (u[i+1] + u[i-1]) + beta * u[i];
        }
    }
    
    //  some outputs and gnuplot scripts
    output(folder, u, time_final);
    gnuplot_onedim(folder, "explicit scheme", time_final);
    gnuplot_onedim_png(folder, "explicit scheme", time_final);
}

void onedim_implicit(const unsigned meshpoints, const double time_final, const unsigned time_steps, const std::string folder)
{
    
    /*
     We want to solve the 1D diffusion equation.
     By scalling and discretizing we come up with a linear algebra system.
     Let a squared (n+1) tridiagonal matrix A with constant diagonals a, b and c.
     Let two vectors u and y.
     We solve A * u = y, y being u at a previous time-step with our solver
    */
    
    const double h = 1. / (double) meshpoints;          //  space-step
    const double dt = time_final / (double) time_steps;
    const double alpha = dt / (h * h);
    vector<double> u(meshpoints + 1);                   //  solution vector
    vector<double> y(meshpoints + 1);
    vector<double> b(meshpoints + 1);                   //  vector of the main diagonal
    
    initial_conditions(u, y);
    
    for(double t = 0.; t < time_final; t += dt)
    {
        //  gaussian elimination, we enter the three diagonals
        tridiagauss(meshpoints, - alpha, 1. + 2 * alpha, - alpha, u, b, y);
        y = u;  //  iteration for the next time-step
    }
    
    //  some outputs and gnuplot scripts
    output(folder, u, time_final);
    gnuplot_onedim(folder, "implicit scheme", time_final);
    gnuplot_onedim_png(folder, "implicit scheme", time_final);
}

void onedim_cranknicolson(const unsigned meshpoints, const double time_final, const unsigned time_steps, const std::string folder)
{
    /*
     We want to solve the 1D diffusion equation.
     By scalling and discretizing we come up with a linear algebra system.
     We first perform a matrix*vector multiplication.
     Then we perform a matrix inversion with the new vector.
     Once again we use our tridiagonal solver.
    */
    
    const double h = 1. / (double) meshpoints;          //  space-step
    const double dt = time_final / (double) time_steps;
    const double alpha = dt / (h * h);        
    const double beta = 2. - 2. * alpha;
    const double gamma = 2. + 2. * alpha;
    vector<double> u(meshpoints + 1);                   //  solution vector
    vector<double> y(meshpoints + 1);
    vector<double> b(meshpoints + 1);                   //  vector of the main diagonal
    
    alpha_warning(alpha, 0.5);
    initial_conditions(u, y);
    
    for(double time = 0.; time < time_final; time += dt)
    {
        //  first we initialize y to use it after in the tridiag solver
        //  we compute (2I - alpha*B)*y and put this new vector as y~
        //  but before the gaussian elimination we have y=u so we use u
        for(unsigned i = 1; i < meshpoints; i++)
        {
            y[i] = alpha * u[i-1] + beta * u[i] + alpha * u[i+1];
        }
        
        //  now we perform the gaussian elimination for (2I + alpha*B)*u = y~
        tridiagauss(meshpoints, - alpha, gamma, - alpha, u, b, y);
        y = u;
    }
    
    //  some outputs and gnuplot scripts
    output(folder, u, time_final);
    gnuplot_onedim(folder, "Crank-Nicolson scheme", time_final);
    gnuplot_onedim_png(folder, "Crank-Nicolson scheme", time_final);
}

void onedim_analytic(const double time_final, const std::string folder)
{
    /*
     Analytical solution for the 1D diffusion equation.
     We sum up the 228 first terms of the fourier series.
    */
    
    const double h = 1. / 5000.;    //  space-step
    vector<double> u;               //  solution vector
    double fourier;                 //  fourier term at each order
    double exponential;
    double sine;
    double ck;                      //  ck coefficient in the fourier term
    
    //  discretization over the space between 0 and 1
    for(double x = 0; x <= 1.; x += h)
    {
        fourier = 0.;
            
        for(unsigned k = 1; k <= 228; k++)
        {
            ck = (k % 2 == 0 ? 2. / ((double) k * M_PI) : - 2. / ((double) k * M_PI));
            exponential = exp(- (double) k * k * M_PI * M_PI * time_final);
            sine        = sin((double) k * M_PI * x);
            
            fourier += ck * exponential * sine;
        }
    
        u.push_back(fourier + x);
    }
    
    //  some outputs and gnuplot scripts
    output(folder, u, time_final);
    gnuplot_onedim(folder, "Analytical solution", time_final);
    gnuplot_onedim_png(folder, "Analytical solution", time_final);
}



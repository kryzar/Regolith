//
//  utilities.hpp
//  Program
//
//  Created by Antoine Hugounet on 24/11/2017.
//

#pragma once

#include <vector>
#include <string>

inline void initial_conditions(std::vector<double>& u);
inline void initial_conditions(std::vector<double>& u, std::vector<double>& y);
inline void initial_conditions(std::vector<std::vector<double>>& u, const unsigned meshpoints);
inline void tridiagauss(const int n, const double a, const double b_val, const double c, std::vector<double>& u, std::vector<double>& b, std::vector<double>& y);

void alpha_warning(const double alpha, const double requirement);
void output(const std::string folder, const std::vector<double>& u, const double time_final);
void output(const std::string folder, const std::vector<std::vector<double>>& u, const double time_final);
void gnuplot_onedim(const std::string folder, const std::string scheme, const double time_final);
void gnuplot_onedim_png(const std::string folder, const std::string scheme, const double time_final);
void gnuplot_twodim(const std::string folder, const std::string scheme, const double time_final);
void gnuplot_twodim_png(const std::string folder, const std::string scheme, const double time_final);



inline void initial_conditions(std::vector<double>& u)
{
    /*
     Initial conditions for the one-dimension system.
    */
    
    fill(u.begin(), u.end() - 1, 0);
    u.back() = 1.;
}

inline void initial_conditions(std::vector<double>& u, std::vector<double>& y)
{
    /*
     Initial conditions for the one-dimension system.
    */
    
    fill(u.begin(), u.end() - 1, 0);
    u.back() = 1.;
    y = u;
}

inline void initial_conditions(std::vector<std::vector<double>>& u, const unsigned meshpoints)
{
    /*
     Initial conditions for the two-dimensions system.
     Feel free to modify it.
    */
    
    for(unsigned i = 0; i < meshpoints + 1; i++)
    {
        u[i][0]             = 1.;  //  down
        u[meshpoints][i]    = 1.;  //  right
        u[i][meshpoints]    = 1.;  //  up
        u[0][i]             = 1.;  //  left
    }
}

inline void tridiagauss(const int meshpoints, const double a, const double b_val, const double c, std::vector<double>& u, std::vector<double>& b, std::vector<double>& y)
{
    //  reset the matrix at each loop, for every scheme we need a power of the same matrix
    
    fill(b.begin(), b.end(), b_val);
    
    //  forward substitution for A*u = y
    //  remember that u(k) in the code is u(k-1) irl
    //  therefore we stop at u(n-1) which is u(n) irl
    //  we do not ever work on the last boundary point of the vector
    
    for(int i = 1; i < meshpoints; i++)
    {
        b[i] -= (a / b[i-1]) * c;
        y[i] -= (a / b[i-1]) * y[i-1];
    }
    
    //  backward substitution
    
    for(int i = meshpoints - 1; i > 0; i--)
    {
        u[i] = (y[i] - c * u[i+1]) / b[i];
    }
}

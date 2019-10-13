//
//  utilities.cpp
//  Program
//
//  Created by Antoine Hugounet on 24/11/2017.
//

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

/*
 Nothing interesting here. Just output functions.
*/

void alpha_warning(const double alpha, const double requirement)
{
    if(alpha > requirement)
    {
        cout << "alpha = (dt / h^2) = " << alpha << endl;
        cout << "We must have (dt / h^2) < " + to_string(requirement) + " for reliable results." << endl;
        exit(1);
    }
}

void output(const std::string folder, const std::vector<double>& u, const double time_final)
{
    ofstream results(folder + "results");
    results << "final time = " << time_final << endl << endl;
    
    for(int i = 0; i < u.size(); i++)
    {
        results << setprecision(8) << (double) i / (u.size() - 1.) << setw(15) << u[i] << endl;
    }
    
    results.close();
}

void output(const std::string folder, const std::vector<std::vector<double>>& u, const double time_final)
{
    ofstream results(folder + "results");
    results << "final time = " << time_final << endl << endl;
    const unsigned long n = u[0].size();
    
    for(int x = 0; x < n; x++)
    {
        for(int y = 0; y < n; y++)
        {
            results << setprecision(3) << (double) x / n << setw(10);
            results << setprecision(3) << (double) y / n << setw(15);
            results << setprecision(8) << u[x][y] << endl;
        }
        
        results << endl;
    }
    
    results.close();
}

void gnuplot_onedim(const std::string folder, const std::string scheme, const double time_final)
{
    ofstream gnuplot(folder + "plot.gnu");
    
    gnuplot << "reset" << endl << endl;
    gnuplot << "set size ratio -1" << endl;
    gnuplot << "set title \"Diffusion equation in one dimension, " + scheme + ", t=" + to_string(time_final) + "\"" << endl;
    gnuplot << "set xlabel \"x\"" << endl;
    gnuplot << "set ylabel \"u(x,t)\"" << endl << endl;
    gnuplot << "plot \"" + folder + "results" + "\" using 1:2 w l notitle" << endl;
    
    gnuplot.close();
}

void gnuplot_onedim_png(const std::string folder, const std::string scheme, const double time_final)
{
    ofstream gnuplot(folder + "plot-png.gnu");
    
    gnuplot << "reset" << endl << endl;
    gnuplot << "set size ratio -1" << endl;
    gnuplot << "data = \"" + folder + "results" + "\"" << endl << endl;
    gnuplot << "set terminal png" << endl;
    gnuplot << "set output \"" + folder + "map " + to_string(time_final) + ".png\"" << endl;
    gnuplot << "set title \"Diffusion equation in one dimension, " + scheme + ", t=" + to_string(time_final) + "\"" << endl;
    gnuplot << "set xlabel \"x\"" << endl;
    gnuplot << "set ylabel \"u(x,t)\"" << endl << endl;
    gnuplot << "plot \"" + folder + "results" + "\" using 1:2 w l notitle" << endl;
    
    gnuplot.close();
}

void gnuplot_twodim(const std::string folder, const std::string scheme, const double time_final)
{
    ofstream gnuplot(folder + "plot.gnu");
    
    gnuplot << "reset" << endl << endl;
    gnuplot << "set size ratio -1" << endl;
    gnuplot << "data = \"" + folder + "results" + "\"" << endl << endl;
    gnuplot << "set title \"Diffusion equation in two dimensions, " + scheme + ", t=" + to_string(time_final) + "\"" << endl;
    gnuplot << "set xlabel \'x\'" << endl;
    gnuplot << "set ylabel \'y\'" << endl;
    gnuplot << "set xrange [0:1]" << endl;
    gnuplot << "set yrange [0:1]" << endl;
    gnuplot << "set view map" << endl;
    gnuplot << "set cblabel \"u(x, y, t)\"" << endl << endl;
    gnuplot << "splot data using 1:2:3 with image notitle" << endl;
    
    gnuplot.close();
}

void gnuplot_twodim_png(const std::string folder, const std::string scheme, const double time_final)
{
    ofstream gnuplot(folder + "plot-png.gnu");
    
    gnuplot << "reset" << endl << endl;
    gnuplot << "set size ratio -1" << endl;
    gnuplot << "data = \"" + folder + "results" + "\"" << endl << endl;
    gnuplot << "set terminal png" << endl;
    gnuplot << "set output \"" + folder + "heat map " + to_string(time_final) + ".png\"" << endl;
    gnuplot << "set title \"Diffusion equation in two dimensions, " + scheme + ", t=" + to_string(time_final) << "\"" << endl;
    gnuplot << "set xlabel \'x\'" << endl;
    gnuplot << "set ylabel \'y\'" << endl;
    gnuplot << "set xrange [0:1]" << endl;
    gnuplot << "set yrange [0:1]" << endl;
    gnuplot << "set view map" << endl;
    gnuplot << "set cblabel \"u(x, y, t)\"" << endl << endl;
    gnuplot << "splot data using 1:2:3 with image notitle" << endl;
    
    gnuplot.close();
}

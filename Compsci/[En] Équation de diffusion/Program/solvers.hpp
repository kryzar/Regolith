//
//  solvers.hpp
//  Program
//
//  Created by Antoine Hugounet on 24/11/2017.
//

#pragma once

#include <string>

void onedim_cranknicolson(const unsigned meshpoints, const double time_final, const unsigned time_steps, const std::string folder);
void onedim_explicit(const unsigned meshpoints, const double time_final, const unsigned time_steps, const std::string folder);
void onedim_implicit(const unsigned meshpoints, const double time_final, const unsigned time_steps, const std::string folder);
void onedim_analytic(const double time_final, const std::string folder);

void twodim_explicit(const unsigned meshpoints, const double time_final, const unsigned time_steps, const std::string folder);
void twodim_implicit(const unsigned meshpoints, const double time_final, const unsigned time_steps, const std::string folder);

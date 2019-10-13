//
//  unit-tests.cpp
//  Program
//
//  Created by Antoine Hugounet on 24/10/2017.
//  Copyright © 2017 Hugounet and Villeneuve. All rights reserved.
//

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "classes/planet.hpp"
#include "classes/solver.hpp"
#include <cmath>

using namespace std;

int run_unittest(int argc, const char* argv[])
{
    
    return (Catch::Session().run(argc, argv));
}

bool equality_small(const double x, const double y)
{
    
    return (abs(x - y) < 1.E-7);
}

bool equality_big(const double x, const double y)
{
    
    return (abs(x - y) < 1.E6);;
}


TEST_CASE("Several minor operations are tested for both planet and solver", "[planet and solver]")
{
   
    planet _earth("earth", 6.E24, 8.30757514E-01, 5.54644964E-01, -9.79193739E-03, 1.42820162E-02);
    planet _jupiter("jupiter", 1.9E27, -4.54463137, -2.98088727, 4.05019642E-03, -5.95135698E-03);
    planet _mars("mars", 6.6E23, -1.60063680E+00, 4.51266379E-01, -3.22884752E-03, -1.22815747E-02);
    planet _sun("sun", 2.E30, 2.17112305E-03, 5.78452455E-03, -5.30635989E-06, 5.44444408E-06);
    planet _sun_masscenter("sun", 1.E23, 0., 0., 0., 0.);
    planet _test("test", 1.E20, 42., 0., 8827., 0.);
    
    solver system;
    
    SECTION("planet::planet(other_planet")
    {
        planet copy = _mars;
        REQUIRE(copy.mass() == _mars.mass());
        REQUIRE(copy.position == _mars.position);
        REQUIRE(copy.velocity == _mars.velocity);
        REQUIRE(copy.kinetic_energy() == _mars.kinetic_energy());
        REQUIRE(copy.potential_energy(_earth) == _mars.potential_energy(_earth));
        REQUIRE(copy.total_energy(_jupiter) == _mars.total_energy(_jupiter));
        REQUIRE(copy.distance_center() == _mars.distance_center());
        REQUIRE(copy.velocity_squared() == _mars.velocity_squared());
    }

    SECTION("planet::distance()")
    {
        double x = 8.30757514E-01 - 2.17112305E-03;
        double y = 5.54644964E-01 - 5.78452455E-03;
        
        REQUIRE(_earth.distance(_sun) == sqrt(x*x + y*y));
        REQUIRE(_test.distance(_sun_masscenter) == 42.);
    }
    
    SECTION("planet::distance_center()")
    {
        double x = -1.60063680E+00;
        double y = 4.51266379E-01;
        
        REQUIRE(_mars.distance_center() == sqrt(x*x + y*y));
        REQUIRE(_test.distance_center() == 42.);
    }
    
    SECTION("planet : energies")
    {
        double kinetic = 0.5 * 1.E20 * 8827 * 8827;
        double potential = 4 * M_PI * M_PI * (1.E20 * 1.E23) / (42.*42.);
        double total = kinetic + potential;
        
        REQUIRE(_test.kinetic_energy() == kinetic);
        REQUIRE(_test.potential_energy(_sun_masscenter) == potential);
        REQUIRE(_test.total_energy(_sun_masscenter) == total);
    }
    
    SECTION("planet::normalize()")
    {
        planet copy_earth = _earth;
        copy_earth.normalize();
        
        REQUIRE(_earth.mass() == 2.E30 * copy_earth.mass());
        //  avoid loss of numerical precision
        REQUIRE(equality_small(_earth.velocity[0], copy_earth.velocity[0] / 365.25));
        REQUIRE(equality_small(_earth.velocity[1], copy_earth.velocity[1] / 365.25));
    }
    
    SECTION("solver::total_mass()")
    {
        double mass =  6.E24 + 1.9E27 + 6.6E23 + 2.E30;
        
        system.add(_earth);
        system.add(_jupiter);
        system.add(_mars);
        system.add(_sun);
        
        REQUIRE(system.total_mass() == mass);
    }
    
    SECTION("solver : energies")
    {
        system.add(_earth);
        planet copy_earth = _earth;
        copy_earth.normalize();

        //  where we see that we have a HUGE loss of numerical precision
        //  as a consequence our energy plots are impacted
        REQUIRE(equality_small(system.kinetic_energy(), copy_earth.kinetic_energy()));
        REQUIRE(system.potential_energy() == _earth.potential_energy(_earth));
        REQUIRE(system.potential_energy() == 0.);
        REQUIRE(equality_small(system.total_energy(), copy_earth.kinetic_energy()));
    }
    
    SECTION("solver::system.total_mass")
    {
        vector<double> position(2);
        double total_mass;
        
        system.add(_earth);
        system.add(_sun_masscenter);
        
        total_mass = system.total_mass();
        
        position[0] = (1 / total_mass) * _earth.mass() * _earth.position[0];
        position[1] = (1 / total_mass) * _earth.mass() * _earth.position[1];
        
        REQUIRE(system.mass_center() == position);
    }
}

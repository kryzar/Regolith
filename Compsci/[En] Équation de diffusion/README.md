# Calypso <br> Project 5 from the course FYS3150 of University of Oslo, Autumn 2017

This program contains finite-differences solvers which solve the diffusion equation in one dimension (*d2u/dx2 = du/dt*) and in two dimensions (*d2u/dx2 + d2u/dy2 = du/dt*). Feel free to click on this very cute hedgehog to see a very nice simulation of the diffusion equation that was computed with this program.

[![hedgehog](https://s33.postimg.org/7dg0c1jnj/67895618herisson7gn-gif.gif)](https://www.youtube.com/watch?v=YKOQDSfEe7k)

## One dimension

The boundary conditions are *u(0,t)=0* and *u(L,t)=1* for any time *t*, that is, at the equilibrium state, the curve is linear and goes from *u(x,t)=x*. You will see that this equilibrium is reached after *t=0.3* roughly. There are three different schemes (see the [report](https://github.com/kryzar/Calypso/blob/master/Report/main.pdf) for full details) :
- An explicit scheme with strong requirements on the values you chose for *dx* and *dt*.
- An implicit scheme without those requirements.
- A Crank-Nicolson scheme, which is the most stable scheme. If on the paper it has no restrictions for dx and *dt*, it is factually not that true and therefore it is wise to use it with the same restrictions that the explicit scheme.

Here is how to use those algorithms :

```cpp
#include "solvers.hpp"
#include <string>

int main()
{
    const string folder = "/Tristan/und/Isolde";

    //  dx = 1/100, final time = 0.2, dt = 1/50000
    onedim_explicit(100, 0.2, 50000, folder);
    //  dx = 1/10, final time = 0.4, dt = 1/500
    onedim_implicit(10, 0.4, 500, folder);
    //  dx = 1/10000, final time = 0.00001, dt = 1/673, but bad choices
    onedim_cranknicolson(10000, 0.00001, 673, folder);

    return 0;
}
```

The explicit scheme only works for *alpha := dt/dx^2 < 1/2*. If the values you enter do not satisfy this requirement, the program will exit. Same for the Crank-Nicolson scheme. The analytical solution has also been coded so that you can compare it to the schemes. It is the partial sum of 228 terms of the Fourier-series solution, with 5000 space-steps.

```cpp
#include "solvers.hpp"
#include <string>

int main()
{
    const string folder = "/I/love/folders/";

    //  final time = 0.5
    onedim_analytical(0.5, folder);

    return 0;
}
```

## Two dimensions

The principle is the same than in one dimension and the implementation is rather easy. However the boundary conditions are left to be decided ; the interior of the lattice is always *0* at *t=0* but you can choose any values you want for the boundaries. The simplest way to modify them is to go to [this file](https://github.com/kryzar/Calypso/blob/master/Program/Program/utilities.hpp) and to directly modify the function `initial_conditions` (you have nothing else to do than putting the values you want here). Each row in the loop stands for a boundary of the squared lattice. You can see different boundary conditions on those [simulations made with this program](https://www.youtube.com/playlist?list=PL9Bkzl2Vcy4sJMAbtl1KsfRhMv7KhHTp6).

This time there are only two schemes :

- An explicit scheme, again with requirements on *dt* and *dx* (this time with *alpha < 1/4*).
- An implicit scheme.

And here is how to use them :

```cpp
#include "solvers.hpp"
#include <string>

int main()
{
    const string folder = "/The/dark/side/of/the/moon";

    //  dx = 1/100, final time = 0.2, dt = 1/50000
    twodim_explicit(100, 0.2, 50000, folder);
    //  dx = 1/10, final time = 0.4, dt = 1/500
    twodim_implicit(10, 0.4, 500, folder);

    return 0;
}
```

## Output files

Each onedim or twodim solver will output three distinct files :
1. a file `results` which contains the values *u(x,t)* (resp. *u(x, y, t)*) for a given *x* (resp. *x* and *y*).
2. a script `plot.gnu` which is to be called in gnuplot like this : `load "plot.gnu"` and that will plot *u*.
3. a script `plot-png.gnu` which is to be called in gnuplot like this : `load "plot-png.gnu"` and that will plot *u* and export the plot in a png-file directly in the folder of the results.

## Warnings and license

It is wise to compare the schemes and to see if they give the same values for *u* within a certain tolerance. However if you choose small values for *alpha* (careful with the loss of numerical precision) there should not be any problem.

Be indulgent - this is a school project - and do what you want, especially reporting comments and mistakes.

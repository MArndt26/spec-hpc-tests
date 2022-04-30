#include "kokkos_shared.hpp"
#include "../../shared.h"

using namespace Kokkos;

// Initialises the Jacobi solver
void jacobi_init(
			const int x, const int y, const int halo_depth, 
            const int coefficient, const double rx, const double ry, KView u, 
            KView u0, KView density, KView energy, KView kx, KView ky) 
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index) 
    {
        const size_t kk = index % x; 
        const size_t jj = index / x; 

        if(kk > 0 && kk < x - 1 && 
           jj > 0 && jj < y - 1)
        {
            u0(index) = energy(index)*density(index);
            u(index) = u0(index);
        }

        if(jj >= halo_depth && jj < y-1 && 
           kk >= halo_depth && kk < x-1)
        {
            double densityCentre = (coefficient == CONDUCTIVITY) 
                ? density(index) : 1.0/density(index);
            double densityLeft = (coefficient == CONDUCTIVITY) 
                ? density(index-1) : 1.0/density(index-1);
            double densityDown = (coefficient == CONDUCTIVITY) 
                ? density(index-x) : 1.0/density(index-x);

            kx(index) = rx*(densityLeft+densityCentre) /
                (2.0*densityLeft*densityCentre);
            ky(index) = ry*(densityDown+densityCentre) /
                (2.0*densityDown*densityCentre);
        }
    });
}

// Main Jacobi solver method.
void jacobi_iterate(
        const int x, const int y, const int halo_depth, KView u, 
        KView u0, KView r, KView kx, KView ky, double* error) 
{
    parallel_reduce(x*y, KOKKOS_LAMBDA (const int index, double& temp_error)
    {
        const size_t kk = index % x; 
        const size_t jj = index / x; 

        if(kk >= halo_depth && kk < x - halo_depth &&
           jj >= halo_depth && jj < y - halo_depth)
        {
            u(index) = (u0(index) 
                + (kx(index+1)*r(index+1) + kx(index)*r(index-1)) 
                + (ky(index+x)*r(index+x) + ky(index)*r(index-x)))
                / (1.0 + (kx(index)+kx(index+1)) + (ky(index)+ky(index+x)));

            temp_error += fabs(u(index)-r(index));
        }
    }, *error);
}

// Copies u into r
void jacobi_copy_u(
        const int x, const int y, KView r, KView u)
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index)
    {
        r(index) = u(index);	
    });
}


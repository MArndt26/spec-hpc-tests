#pragma once
#include "kokkos_shared.hpp"
#include "../../shared.h"

/*
 *		JACOBI SOLVER KERNEL
 */

// Initialises the Jacobi solver
template <class Device>
struct JacobiInit
{
	typedef Device device_type;

	JacobiInit(
			const int x, const int y, const int halo_depth, 
            const int coefficient, const double rx, const double ry, KView u, 
            KView u0, KView density, KView energy, KView kx, KView ky) 
		: x(x), y(y), halo_depth(halo_depth), u(u), u0(u0), 
        density(density), energy(energy), kx(kx), ky(ky), 
        coefficient(coefficient), rx(rx), ry(ry) {}

	KOKKOS_INLINE_FUNCTION
	void operator()(const int index) const 
    {
        const int kk = index % x; 
        const int jj = index / x; 

        if(kk > 0 && kk < x - 1 && jj > 0 && jj < y - 1)
        {
            u0(index) = energy(index)*density(index);
            u(index) = u0(index);
        }

        if(jj >= halo_depth && jj < y-1 && kk >= halo_depth && kk < x-1)
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
    }

    const int x;
    const int y;
    const int halo_depth;
    const int coefficient;
    const double rx;
    const double ry;
    KView u;
    KView u0;
    KView density;
    KView energy;
    KView kx;
    KView ky;
};

// Copies u into r
template <class Device>
struct JacobiCopyU
{
    typedef Device device_type;

    JacobiCopyU(KView r, KView u) 
        : r(r), u(u){}

    KOKKOS_INLINE_FUNCTION
        void operator()(const int index) const 
        {
            r(index) = u(index);	
        }

    KView r;
    KView u;
};

// Main Jacobi solver method.
template <class Device>
struct JacobiIterate
{
    typedef double value_type;
    typedef Device device_type;
    typedef Kokkos::View<double*,Device> KView;

    JacobiIterate(const int x, const int y, const int halo_depth, KView u, 
            KView u0, KView r, KView kx, KView ky) 
        : x(x), y(y), halo_depth(halo_depth), u(u), u0(u0), 
        r(r), kx(kx), ky(ky) {}

    KOKKOS_INLINE_FUNCTION
        void operator()(const int index, double& error) const 
        {
            const int kk = index % x; 
            const int jj = index / x; 

            if(kk >= halo_depth && kk < x - halo_depth &&
                    jj >= halo_depth && jj < y - halo_depth)
            {
                u(index) = (u0(index) 
                        + (kx(index+1)*r(index+1) 
                            + kx(index)*r(index-1))
                        + (ky(index+x)*r(index+x) 
                            + ky(index)*r(index-x)))
                    / (1.0 + (kx(index)+kx(index+1)) 
                            + (ky(index)+ky(index+x)));

                error += fabs(u(index)-r(index));
            }
        }

    const int x;
    const int y;
    const int halo_depth;
    KView u;
    KView u0;
    KView r;
    KView kx;
    KView ky;
};


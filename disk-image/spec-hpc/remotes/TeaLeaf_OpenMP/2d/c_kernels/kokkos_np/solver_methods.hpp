#pragma once
#include "kokkos_shared.hpp"
#include "../../shared.h"

// Copies the inner u into u0.
template <class Device>
struct CopyU
{
    typedef Device device_type;

    CopyU(const int x, const int y, const int halo_depth, KView u, KView u0) 
        : x(x), y(y), halo_depth(halo_depth), u(u), u0(u0){}

    KOKKOS_INLINE_FUNCTION
        void operator()(const int index) const 
        {
            const int kk = index % x; 
            const int jj = index / x; 

            if(kk >= halo_depth && kk < x - halo_depth &&
                    jj >= halo_depth && jj < y - halo_depth)
            {
                u0(index) = u(index);	
            }
        }

    const int x;
    const int y;
    const int halo_depth;
    KView u;
    KView u0;
};

// Calculates the residual r.
template <class Device>
struct CalculateResidual
{
    typedef Device device_type;

    CalculateResidual(
            const int x, const int y, const int halo_depth, KView u, 
            KView u0, KView r, KView kx, KView ky) 
        : x(x), y(y), halo_depth(halo_depth), u(u), u0(u0), 
        r(r), kx(kx), ky(ky){}

    KOKKOS_INLINE_FUNCTION
        void operator()(const int index) const 
        {
            const int kk = index % x; 
            const int jj = index / x; 

            if(kk >= halo_depth && kk < x - halo_depth &&
                    jj >= halo_depth && jj < y - halo_depth)
            {
                const double smvp = SMVP(u);
                r(index) = u0(index) - smvp;
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

// Calculates the 2 norm of the provided buffer.
template <class Device>
struct Calculate2Norm
{
    typedef Device device_type;
    typedef double value_type;

    Calculate2Norm(const int x, const int y, const int halo_depth, KView buffer) 
        : x(x), y(y), halo_depth(halo_depth), buffer(buffer){}

    KOKKOS_INLINE_FUNCTION
        void operator()(const int index, double& norm) const 
        {
            const int kk = index % x; 
            const int jj = index / x; 

            if(kk >= halo_depth && kk < x - halo_depth &&
                    jj >= halo_depth && jj < y - halo_depth)
            {
                norm += buffer(index)*buffer(index);			
            }
        }

    const int x;
    const int y;
    const int halo_depth;
    KView buffer;
};

// Finalises the energy field.
template <class Device>
struct Finalise
{
    typedef Device device_type;

    Finalise(const int x, const int y, const int halo_depth, KView u, 
            KView density, KView energy) 
        : x(x), y(y), halo_depth(halo_depth), u(u), 
        density(density), energy(energy) {}

    KOKKOS_INLINE_FUNCTION
        void operator()(const int index) const 
        {
            const int kk = index % x; 
            const int jj = index / x; 

            if(kk >= halo_depth && kk < x - halo_depth &&
                    jj >= halo_depth && jj < y - halo_depth)
            {
                energy(index) = u(index)/density(index);
            }
        }

    const int x;
    const int y;
    const int halo_depth;
    KView u;
    KView density;
    KView energy;
};


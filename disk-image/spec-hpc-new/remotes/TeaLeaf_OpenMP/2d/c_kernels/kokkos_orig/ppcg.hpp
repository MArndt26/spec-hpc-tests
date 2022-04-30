#pragma once
#include "kokkos_shared.hpp"
#include "../../shared.h"

/*
 *		PPCG SOLVER KERNEL
 */

// Initialises Sd
template <class Device>
struct PPCGInit
{
	typedef Device device_type;

	PPCGInit(const int x, const int y, const int halo_depth, const double theta, 
            KView sd, KView r) 
		: x(x), y(y), halo_depth(halo_depth), sd(sd), r(r), 
        theta(theta) {}

	KOKKOS_INLINE_FUNCTION
	void operator()(const int index) const 
    {
        const int kk = index % x; 
        const int jj = index / x; 

        if(kk >= halo_depth && kk < x - halo_depth &&
           jj >= halo_depth && jj < y - halo_depth)
        {
            sd[index] = r[index]/theta;
        }
    }

    const int x;
    const int y;
    const int halo_depth;
    const double theta; 
    KView sd; 
    KView r; 
};

// Calculates U and R
template <class Device>
struct PPCGCalcUR
{
    typedef Device device_type;

    PPCGCalcUR(const int x, const int y, const int halo_depth, KView sd, 
            KView r, KView u, KView kx, KView ky) 
        : x(x), y(y), halo_depth(halo_depth), sd(sd), r(r), 
        u(u), kx(kx), ky(ky) {}

    KOKKOS_INLINE_FUNCTION
        void operator()(const int index) const 
        {
            const int kk = index % x; 
            const int jj = index / x; 

            if(kk >= halo_depth && kk < x - halo_depth &&
                    jj >= halo_depth && jj < y - halo_depth)
            {
                const double smvp = SMVP(sd);
                r[index] -= smvp;
                u[index] += sd[index];
            }
        }

    const int x;
    const int y;
    const int halo_depth;
    KView sd; 
    KView r; 
    KView u; 
    KView kx; 
    KView ky; 
};

// Calculates Sd
template <class Device>
struct PPCGCalcSd
{
    typedef Device device_type;

    PPCGCalcSd(const int x, const int y, const int halo_depth, const double theta, 
            const double alpha, const double beta, KView sd, KView r) 
        : x(x), y(y), halo_depth(halo_depth), sd(sd), r(r), 
        alpha(alpha), beta(beta), theta(theta) {}

    KOKKOS_INLINE_FUNCTION
        void operator()(const int index) const 
        {
            const int kk = index % x; 
            const int jj = index / x; 

            if(kk >= halo_depth && kk < x - halo_depth &&
                    jj >= halo_depth && jj < y - halo_depth)
            {
                sd[index] = alpha*sd[index] + beta*r[index];
            }
        }

    const int x;
    const int y;
    const int halo_depth;
    const double alpha;
    const double beta;
    const double theta; 
    KView sd; 
    KView r; 
};


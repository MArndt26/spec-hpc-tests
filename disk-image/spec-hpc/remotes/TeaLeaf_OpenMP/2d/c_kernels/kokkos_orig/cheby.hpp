#pragma once
#include "kokkos_shared.hpp"

// Initialises the Cheby solver
template <class Device>
struct ChebyInit
{
	typedef Device device_type;

	ChebyInit(const int x, const int y, const int halo_depth, const double theta, 
            KView p, KView r, KView u, KView u0, KView w, 
            KView kx, KView ky) 
		: x(x), y(y), halo_depth(halo_depth), p(p), r(r), 
        u(u), u0(u0), w(w), kx(kx), 
        ky(ky), theta(theta) {}

	KOKKOS_INLINE_FUNCTION
	void operator()(const int index) const 
    {
        const int kk = index % x; 
        const int jj = index / x; 

        if(kk >= halo_depth && kk < x - halo_depth &&
           jj >= halo_depth && jj < y - halo_depth)
        {
            const double smvp = SMVP(u);
            w[index] = smvp;
            r[index] = u0[index]-w[index];
            p[index] = r[index]/theta;
        }
    }

    const int x;
    const int y;
    const int halo_depth;
    const double theta; 
    KView p; 
    KView r; 
    KView u; 
    KView u0; 
    KView w; 
    KView kx; 
    KView ky; 
};

// Calculates U
template <class Device>
struct ChebyCalcU
{
    typedef Device device_type;

    ChebyCalcU(const int x, const int y, const int halo_depth, KView p, 
            KView u) 
        : x(x), y(y), halo_depth(halo_depth), p(p), u(u) {}

    KOKKOS_INLINE_FUNCTION
        void operator()(const int index) const 
        {
            const int kk = index % x; 
            const int jj = index / x; 

            if(kk >= halo_depth && kk < x - halo_depth &&
                    jj >= halo_depth && jj < y - halo_depth)
            {
                u[index] += p[index];
            }
        }

    const int x;
    const int y;
    const int halo_depth;
    KView p; 
    KView u; 
};

// The main Cheby iteration step
template <class Device>
struct ChebyIterate
{
    typedef Device device_type;

    ChebyIterate(const int x, const int y, const int halo_depth, 
            const double alpha, const double beta, KView p, 
            KView r, KView u, KView u0, KView w, KView kx, 
            KView ky) 
        : x(x), y(y), halo_depth(halo_depth), p(p), r(r), 
        u(u), u0(u0), w(w), kx(kx), 
        ky(ky), alpha(alpha), beta(beta) {}

    KOKKOS_INLINE_FUNCTION
        void operator()(const int index) const 
        {
            const int kk = index % x; 
            const int jj = index / x; 

            if(kk >= halo_depth && kk < x - halo_depth &&
               jj >= halo_depth && jj < y - halo_depth)
            {
                const double smvp = SMVP(u);
                w[index] = smvp;
                r[index] = u0[index]-w[index];
                p[index] = alpha*p[index] + beta*r[index];
            }
        }

    const int x;
    const int y;
    const int halo_depth;
    const double alpha; 
    const double beta; 
    KView p; 
    KView r; 
    KView u; 
    KView u0; 
    KView w; 
    KView kx; 
    KView ky; 
};


#pragma once 
#include "kokkos_shared.hpp"

// Calculates key values from the current field.
template <class Device>
struct FieldSummary
{
	typedef Device device_type;
	typedef Kokkos::View<double*,Device> KView;
	typedef struct 
    { 
		double vol; 
		double mass;
		double ie;
		double temp;
	} value_type;

	FieldSummary(
            const int x, const int y, const int halo_depth, KView u, 
            KView density, KView energy0, KView volume)
		: x(x), y(y), halo_depth(halo_depth), u(u), density(density), 
        energy0(energy0), volume(volume) {};

	KOKKOS_INLINE_FUNCTION
	void operator()(int index, value_type& update) const 
    {
        const int kk = index % x; 
        const int jj = index / x; 

        if(kk >= halo_depth && kk < x-halo_depth && 
           jj >= halo_depth && jj < y-halo_depth) 
        {
            const double cellVol = volume[index];
            const double cellMass = cellVol*density[index];
            update.vol += cellVol;
            update.mass += cellMass;
            update.ie += cellMass*energy0[index];
            update.temp += cellMass*u[index];
        }
    }

    KOKKOS_INLINE_FUNCTION
    static void join(volatile value_type& update, const volatile value_type& input)
    {
        update.vol += input.vol;
        update.mass += input.mass;
        update.ie += input.ie;
        update.temp += input.temp;
    }

    KOKKOS_INLINE_FUNCTION
    static void init(value_type& update)
    {
        update.vol = 0.0;
        update.mass = 0.0;
        update.ie = 0.0;
        update.temp = 0.0;
    }

    const int x;
    const int y;
    const int halo_depth;
    KView u;
    KView density;
    KView energy0;
    KView volume;
};


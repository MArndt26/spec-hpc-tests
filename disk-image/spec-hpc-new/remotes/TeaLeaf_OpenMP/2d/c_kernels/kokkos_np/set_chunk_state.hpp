#pragma once
#include "kokkos_shared.hpp"
#include "../../shared.h"

template <class Device>
struct SetChunkInitialState
{
	typedef Device device_type;
	
    SetChunkInitialState(
            double default_energy, double default_density, 
            KView energy0, KView density)
        : default_energy(default_energy), default_density(default_density),
        energy0(energy0), density(density) {}

    KOKKOS_INLINE_FUNCTION
    void operator()(const int index) const
    {
        energy0(index) = default_energy;
        density(index) = default_density;
    }

    double default_energy;
    double default_density;
	KView energy0; 
	KView density; 
};

template <class Device>
struct SetChunkState
{
	typedef Device device_type;

	SetChunkState(
			const int x, const int y, const int halo_depth, State state, 
            KView energy0, KView density, KView u, KView cell_x, KView cell_y, 
            KView vertex_x, KView vertex_y) 
        : x(x), y(y), halo_depth(halo_depth), state(state), energy0(energy0), 
        density(density), u(u), cell_x(cell_x), cell_y(cell_y), 
        vertex_x(vertex_x), vertex_y(vertex_y) {}

	KOKKOS_INLINE_FUNCTION
	void operator()(const int index) const 
    {
        const int kk = index % x; 
        const int jj = index / x; 

		bool applyState = false;

        if(state.geometry == RECTANGULAR) // Rectangular state
        {
            applyState = (
                    vertex_x(kk+1) >= state.x_min && 
                    vertex_x(kk) < state.x_max  &&
                    vertex_y(jj+1) >= state.y_min &&
                    vertex_y(jj) < state.y_max);
        }
        else if(state.geometry == CIRCULAR) // Circular state
        {
            double radius = sqrt(
                    (cell_x(kk)-state.x_min)*(cell_x(kk)-state.x_min)+
                    (cell_y(jj)-state.y_min)*(cell_y(jj)-state.y_min));

            applyState = (radius <= state.radius);
        }
        else if(state.geometry == POINT) // Point state
        {
            applyState = (
                    vertex_x(kk) == state.x_min &&
                    vertex_y(jj) == state.y_min);
        }

        // Check if state applies at this vertex, and apply
        if(applyState)
        {
            energy0(index) = state.energy;
            density(index) = state.density;
        }

        if(kk > 0 && kk < x-1 && jj > 0 && jj < y-1) 
        {
            u(index) = energy0(index)*density(index);
        }
    }

    const int x;
    const int y;
    const int halo_depth;
    State state;
    KView energy0; 
    KView density; 
    KView u; 
    KView cell_x; 
    KView cell_y; 
    KView vertex_x; 
    KView vertex_y; 
};


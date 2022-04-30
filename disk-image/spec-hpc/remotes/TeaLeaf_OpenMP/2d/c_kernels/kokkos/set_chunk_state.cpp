#include "kokkos_shared.hpp"
#include "../../shared.h"
#include "../../settings.h"

using namespace Kokkos;

// Sets the initial state for the chunk
void set_chunk_initial_state(
            const int x, const int y, double default_energy, 
            double default_density, KView energy0, KView density)
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index)
    {
        energy0(index) = default_energy;
        density(index) = default_density;
    });
}

// Sets all of the additional states in order
void set_chunk_state(
			const int x, const int y, const int halo_depth, State state, 
            KView energy0, KView density, KView u, KView cell_x, KView cell_y, 
            KView vertex_x, KView vertex_y) 
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index)
    {
        const size_t kk = index % x; 
        const size_t jj = index / x; 

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
    });
}

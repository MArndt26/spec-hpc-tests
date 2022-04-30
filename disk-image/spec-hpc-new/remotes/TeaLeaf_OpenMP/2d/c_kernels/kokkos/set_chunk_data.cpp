#include "kokkos_shared.hpp"
#include "../../settings.h"
#include "../../shared.h"

using namespace Kokkos;

// Initialises the vertices
void set_chunk_data_vertices(
			const int x, const int y, const int halo_depth, KView vertex_x, 
            KView vertex_y, const double x_min, const double y_min, 
            const double dx, const double dy) 
{
    parallel_for(MAX(x,y)+1, KOKKOS_LAMBDA (const int index)
    {
        if(index < x+1)
        {
			vertex_x(index) = x_min+dx*(index-halo_depth);
        }

		if(index < y+1)
        {
			vertex_y(index) = y_min+dy*(index-halo_depth);
        }
    });
}

// Sets all of the cell data for a chunk
void set_chunk_data(
        const int x, const int y, const int halo_depth, KView vertex_x, 
        KView vertex_y, KView cell_x, KView cell_y, KView volume, 
        KView x_area, KView y_area, const double x_min, const double y_min, 
        const double dx, const double dy) 
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index)
    {
        if(index < x)
        {
            cell_x(index) = 0.5*(vertex_x(index)+vertex_x(index+1));
        }

        if(index < y)
        {
            cell_y(index) = 0.5*(vertex_y(index)+vertex_y(index+1));
        }

        if(index < x*y) {
         volume(index) = dx*dy;
         x_area(index) = dy;
         y_area(index) = dx;
        }
    });
}

#pragma once
#include "kokkos_shared.hpp"
#include "../../settings.h"
#include "../../shared.h"

// Initialises the vertices
template <class Device>
struct SetChunkDataVertices
{
	typedef Device device_type;

	SetChunkDataVertices(
			const int x, const int y, const int halo_depth, KView vertex_x, 
            KView vertex_y, const double x_min, const double y_min, 
            const double dx, const double dy) 
		: x(x), y(y), halo_depth(halo_depth), vertex_x(vertex_x), 
        vertex_y(vertex_y), x_min(x_min), y_min(y_min), dx(dx), dy(dy) {} 

	KOKKOS_INLINE_FUNCTION
	void operator() (const int index) const 
    {
        if(index < x+1)
        {
			vertex_x(index) = x_min+dx*(index-halo_depth);
        }

		if(index < y+1)
        {
			vertex_y(index) = y_min+dy*(index-halo_depth);
        }
    }

    const int x;
    const int y;
    const int halo_depth;
    const double x_min;
    const double y_min;
    const double dx;
    const double dy;
    KView vertex_x;
    KView vertex_y;
};

template <class Device>
struct SetChunkData
{
	typedef Device device_type;

	SetChunkData(
			const int x, const int y, const int halo_depth, KView vertex_x, 
            KView vertex_y, KView cell_x, KView cell_y, KView volume, 
            KView x_area, KView y_area, const double x_min, const double y_min, 
            const double dx, const double dy) 
		: x(x), y(y), halo_depth(halo_depth), vertex_x(vertex_x), 
        vertex_y(vertex_y), cell_x(cell_x), cell_y(cell_y), volume(volume), 
        x_area(x_area), y_area(y_area), x_min(x_min), y_min(y_min), 
        dx(dx), dy(dy) {} 

    KOKKOS_INLINE_FUNCTION
    void operator() (const int index) const 
    {
        if(index < x)
        {
            cell_x(index) = 0.5*(vertex_x(index)+vertex_x(index+1));
        }

        if(index < y)
        {
            cell_y(index) = 0.5*(vertex_y(index)+vertex_y(index+1));
        }

        volume(index) = dx*dy;
        x_area(index) = dy;
        y_area(index) = dx;
    }

    const int x;
    const int y;
    const int halo_depth;
    const double x_min;
    const double y_min;
    const double dx;
    const double dy;
    KView vertex_x;
    KView vertex_y;
    KView cell_x;
    KView cell_y;
    KView volume;
    KView x_area;
    KView y_area;
};


#include "../../shared.h"

/*
 * 		SET CHUNK DATA KERNEL
 * 		Initialises the chunk's mesh data.
 */

// Extended kernel for the chunk initialisation
void set_chunk_data( 
        settings_t* settings,
        int x,
        int y,
        int z, 
        int left,
        int bottom,
        int back,
 	    double* cell_x,
		double* cell_y,
		double* cell_z,
		double* vertex_x,
		double* vertex_y,
		double* vertex_z,
		double* volume,
		double* x_area,
		double* y_area,
		double* z_area)
{
    double x_min = settings->grid_x_min + settings->dx*(double)left;
    double y_min = settings->grid_y_min + settings->dy*(double)bottom;
    double z_min = settings->grid_z_min + settings->dz*(double)back;

	for(int ii = 0; ii < x+1; ++ii)
	{
		vertex_x[ii] = x_min + settings->dx*(ii-settings->halo_depth);
	}

	for(int ii = 0; ii < y+1; ++ii)
	{
		vertex_y[ii] = y_min + settings->dy*(ii-settings->halo_depth);
	}

	for(int ii = 0; ii < z+1; ++ii)
	{
		vertex_z[ii] = z_min + settings->dz*(ii-settings->halo_depth);
	}

	for(int ii = 0; ii < x; ++ii)
	{
		cell_x[ii] = 0.5*(vertex_x[ii]+vertex_x[ii+1]);
	}

	for(int ii = 0; ii < y; ++ii)
	{
		cell_y[ii] = 0.5*(vertex_y[ii]+vertex_y[ii+1]);
	}

	for(int ii = 0; ii < z; ++ii)
	{
		cell_z[ii] = 0.5*(vertex_z[ii]+vertex_z[ii+1]);
	}

	for(int ii = 0; ii < x*y*z; ++ii)
	{
		volume[ii] = settings->dx*settings->dy*settings->dz;
		x_area[ii] = settings->dy*settings->dz;
		y_area[ii] = settings->dx*settings->dz;
		z_area[ii] = settings->dx*settings->dy;
	}
}


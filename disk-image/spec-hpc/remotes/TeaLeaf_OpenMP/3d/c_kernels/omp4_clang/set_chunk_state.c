#include <math.h>
#include "../../settings.h"

/*
 *      SET CHUNK STATE KERNEL
 *		Sets up the chunk geometry.
 */

// Entry point for set chunk state kernel
void set_chunk_state(
        int x,
        int y,
        int z,
        double* vertex_x,
        double* vertex_y,
        double* vertex_z,
        double* cell_x,
        double* cell_y,
        double* cell_z,
        double* density,
        double* energy0,
        double* vec_u,
        const int num_states,
        state_t* states)
{
    // Set the initial state
    for(int ii = 0; ii != x*y*z; ++ii)
    {
        energy0[ii] = states[0].energy;
        density[ii] = states[0].density;
    }	

    // Apply all of the states in turn
    for(int ss = 1; ss < num_states; ++ss)
    {
        for(int ii = 0; ii != z; ++ii) 
        {
            for(int jj = 0; jj != y; ++jj) 
            {
                for(int kk = 0; kk != x; ++kk) 
                {
                    int apply_state = 0;

                    if(states[ss].geometry == CUBOID)
                    {
                        apply_state = (
                                vertex_x[kk+1] >= states[ss].x_min && 
                                vertex_x[kk] < states[ss].x_max    &&
                                vertex_y[jj+1] >= states[ss].y_min &&
                                vertex_y[jj] < states[ss].y_max    &&
                                vertex_z[ii+1] >= states[ss].z_min &&
                                vertex_z[ii] < states[ss].z_max);
                    }
                    else if(states[ss].geometry == SPHERICAL)
                    {
                        double radius = sqrt(
                                (cell_x[kk]-states[ss].x_min)*
                                (cell_x[kk]-states[ss].x_min)+
                                (cell_y[jj]-states[ss].y_min)*
                                (cell_y[jj]-states[ss].y_min)+
                                (cell_z[ii]-states[ss].z_min)*
                                (cell_z[ii]-states[ss].z_min));

                        apply_state = (radius <= states[ss].radius);
                    }
                    else if(states[ss].geometry == POINT)
                    {
                        apply_state = (
                                vertex_x[kk] == states[ss].x_min &&
                                vertex_y[jj] == states[ss].y_min &&
                                vertex_z[ii] == states[ss].z_min);
                    }

                    // Check if state applies at this vertex, and apply
                    if(apply_state)
                    {
                        int index = ii*y*x+jj*x+kk;
                        energy0[index] = states[ss].energy;
                        density[index] = states[ss].density;
                   }
                }
            }
        }
    }

    // Set an initial state for u
    for(int ii = 1; ii != z-1; ++ii) 
    {
        for(int jj = 1; jj != y-1; ++jj) 
        {
            for(int kk = 1; kk != x-1; ++kk) 
            {
                int index = ii*y*x+jj*x+kk;
                vec_u[index] = energy0[index]*density[index];
            }
        }
    }
}


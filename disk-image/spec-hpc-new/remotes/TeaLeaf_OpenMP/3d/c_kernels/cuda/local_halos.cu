#include <stdlib.h>
#include "../../shared.h"
#include "local_halos.cuknl"

/*
 * 		LOCAL HALOS KERNEL
 */	

// The kernel for updating halos locally
void local_halos(
        const int x,
        const int y,
        const int z,
        const int depth,
        const int halo_depth,
        const int* chunk_neighbours,
        const bool* fields_to_exchange,
        double* density,
        double* energy0,
        double* energy,
        double* vec_u,
        double* vec_p,
        double* vec_sd)
{
#define LAUNCH_UPDATE(index, buffer)\
    if(fields_to_exchange[index])\
    {\
        update_face(x, y, z, halo_depth, chunk_neighbours, depth, buffer);\
    }

    LAUNCH_UPDATE(FIELD_DENSITY, density);
    LAUNCH_UPDATE(FIELD_P, vec_p);
    LAUNCH_UPDATE(FIELD_ENERGY0, energy0);
    LAUNCH_UPDATE(FIELD_ENERGY1, energy);
    LAUNCH_UPDATE(FIELD_U, vec_u);
    LAUNCH_UPDATE(FIELD_SD, vec_sd);
#undef LAUNCH_UPDATE
}

// Updates faces in turn.
void update_face(
        const int x,
        const int y, 
        const int z,
        const int halo_depth,
        const int* chunk_neighbours,
        const int depth,
        double* buffer)
{
    const int x_inner = x - 2*halo_depth;
    const int y_inner = y - 2*halo_depth;
    const int z_inner = z - 2*halo_depth;

#define UPDATE_FACE(face, update_kernel) \
    if(chunk_neighbours[face] == EXTERNAL_FACE) \
    {\
        update_kernel<<<num_blocks, BLOCK_SIZE>>>( \
                x, y, z, halo_depth, depth, buffer); \
    }

    int num_blocks = ceil((x_inner*z_inner*depth) / (double)BLOCK_SIZE);
    UPDATE_FACE(CHUNK_TOP, update_top);
    UPDATE_FACE(CHUNK_BOTTOM, update_bottom);

    num_blocks = ceil((x_inner*y_inner*depth) / (double)BLOCK_SIZE);
    UPDATE_FACE(CHUNK_FRONT, update_front);
    UPDATE_FACE(CHUNK_BACK, update_back);

    num_blocks = ceil((y_inner*z_inner*depth) / (double)BLOCK_SIZE);
    UPDATE_FACE(CHUNK_LEFT, update_left);
    UPDATE_FACE(CHUNK_RIGHT, update_right);
}


#include <stdlib.h>
#include "raja_shared.hpp"
#include "../../shared.h"

using namespace RAJA;

void update_left(RAJALists* raja_lists, const int x, const int y, 
        const int halo_depth, const int depth, double* buffer);
void update_right(RAJALists* raja_lists, const int x, const int y, 
        const int halo_depth, const int depth, double* buffer);
void update_top(RAJALists* raja_lists, const int x, const int y, 
        const int halo_depth, const int depth, double* buffer); 
void update_bottom(RAJALists* raja_lists, const int x, const int y, 
        const int halo_depth, const int depth, double* buffer);
void update_face(RAJALists* raja_lists, const int x, const int y, 
        const int halo_depth, const int* chunk_neighbours, const int depth, 
        double* buffer);

// The kernel for updating halos locally
void local_halos(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int depth,
        const int halo_depth,
        const int* chunk_neighbours,
        const bool* fields_to_exchange,
        double* density,
        double* energy0,
        double* energy,
        double* u,
        double* p,
        double* sd)
{
#define LAUNCH_UPDATE(index, buffer)\
    if(fields_to_exchange[index])\
    {\
        update_face(\
                raja_lists, x, y, halo_depth, chunk_neighbours, depth, buffer); \
    }

    LAUNCH_UPDATE(FIELD_DENSITY, density);
    LAUNCH_UPDATE(FIELD_P, p);
    LAUNCH_UPDATE(FIELD_ENERGY0, energy0);
    LAUNCH_UPDATE(FIELD_ENERGY1, energy);
    LAUNCH_UPDATE(FIELD_U, u);
    LAUNCH_UPDATE(FIELD_SD, sd);
}

// Updates faces in turn.
void update_face(
        RAJALists* raja_lists,
        const int x,
        const int y, 
        const int halo_depth,
        const int* chunk_neighbours,
        const int depth,
        double* buffer)
{
#define UPDATE_FACE(face, update_kernel) \
    if(chunk_neighbours[face] == EXTERNAL_FACE)\
    {\
        update_kernel(raja_lists, x, y, halo_depth, depth, buffer);\
    }

    UPDATE_FACE(CHUNK_LEFT, update_left);
    UPDATE_FACE(CHUNK_RIGHT, update_right);
    UPDATE_FACE(CHUNK_TOP, update_top);
    UPDATE_FACE(CHUNK_BOTTOM, update_bottom);
}

// Update left halo.
void update_left(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        const int depth, 
        double* buffer)
{
    forall<policy>(
            raja_lists->lr_halo_list[depth-1], [=] RAJA_DEVICE (int index) 
    {
        const int flip = index % depth;
        const int lines = index / depth;
        const int offset = halo_depth + lines*(x - depth);
        const int from_index = offset + index;
        const int to_index = from_index - (1 + flip*2);
        buffer[to_index] = buffer[from_index];
    });
}

// Update right halo.
void update_right(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        const int depth,
        double* buffer)
{
    forall<policy>(
            raja_lists->lr_halo_list[depth-1], [=] RAJA_DEVICE (int index) 
    {
        const int flip = index % depth;
        const int lines = index / depth;
        const int offset = x - halo_depth + lines*(x - depth);
        const int to_index = offset + index;
        const int from_index = to_index - (1 + flip*2);
        buffer[to_index] = buffer[from_index];
    });
}

// Update top halo.
void update_top(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        const int depth, 
        double* buffer)
{
    forall<policy>(
            raja_lists->tb_halo_list[depth-1], [=] RAJA_DEVICE (int index) 
    {
        const int lines = index/x;
        const int offset = x*(y - halo_depth);
        const int to_index = offset + index;
        const int from_index = to_index - (1 + lines*2)*x;
        buffer[to_index] = buffer[from_index];
    });
}

// Updates bottom halo.
void update_bottom(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        const int depth, 
        double* buffer)
{
    forall<policy>(
            raja_lists->tb_halo_list[depth-1], [=] RAJA_DEVICE (int index) 
    {
        const int lines = index/x;
        const int offset = x*halo_depth;
        const int from_index = offset + index;
        const int to_index = from_index - (1 + lines*2)*x;
        buffer[to_index] = buffer[from_index];
    });
}


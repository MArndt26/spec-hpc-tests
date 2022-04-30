#include <stdlib.h>
#include "../../shared.h"

/*
 * 		LOCAL HALOS KERNEL
 */	

void update_left(const int x, const int y, const int z, 
        const int halo_depth, const int depth, double* buffer);
void update_right(const int x, const int y, const int z, 
        const int halo_depth, const int depth, double* buffer);
void update_top(const int x, const int y, const int z, 
        const int halo_depth, const int depth, double* buffer); 
void update_bottom(const int x, const int y, const int z, 
        const int halo_depth, const int depth, double* buffer);
void update_front(const int x, const int y, const int z, 
        const int halo_depth, const int depth, double* buffer);
void update_back(const int x, const int y, const int z, 
        const int halo_depth, const int depth, double* buffer);
void update_face(const int x, const int y, const int z, const int halo_depth, 
        const int* chunk_neighbours, const int depth, double* buffer);

typedef void (*update_kernel)(int,double*);

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
        double* u,
        double* p,
        double* sd)
{
#define LAUNCH_UPDATE(index, buffer)\
    if(fields_to_exchange[index])\
    {\
        update_face(x, y, z, halo_depth, chunk_neighbours, depth, buffer);\
    }

    LAUNCH_UPDATE(FIELD_DENSITY, density);
    LAUNCH_UPDATE(FIELD_P, p);
    LAUNCH_UPDATE(FIELD_ENERGY0, energy0);
    LAUNCH_UPDATE(FIELD_ENERGY1, energy);
    LAUNCH_UPDATE(FIELD_U, u);
    LAUNCH_UPDATE(FIELD_SD, sd);
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
#define UPDATE_FACE(face, updateKernel) \
    if(chunk_neighbours[face] == EXTERNAL_FACE)\
    {\
        updateKernel(x, y, z, halo_depth, depth,buffer);\
    }

    UPDATE_FACE(CHUNK_LEFT, update_left);
    UPDATE_FACE(CHUNK_RIGHT, update_right);
    UPDATE_FACE(CHUNK_TOP, update_top);
    UPDATE_FACE(CHUNK_BOTTOM, update_bottom);
    UPDATE_FACE(CHUNK_FRONT, update_front);
    UPDATE_FACE(CHUNK_BACK, update_back);
}

// Update left halo.
void update_left(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        const int depth, 
        double* buffer)
{
#pragma omp parallel for
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = 0; kk < depth; ++kk)
            {
                int base = ii*x*y+jj*x;
                buffer[base+(halo_depth-kk-1)] = buffer[base+(halo_depth+kk)];			
            }
        }
    }
}

// Update right halo.
void update_right(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        const int depth,
        double* buffer)
{
#pragma omp parallel for
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = 0; kk < depth; ++kk)
            {
                int base = ii*x*y+jj*x;
                buffer[base+(x-halo_depth+kk)] 
                    = buffer[base+(x-halo_depth-1-kk)];
            }
        }
    }
}

// Update top halo.
void update_top(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        const int depth, 
        double* buffer)
{
#pragma omp parallel for
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = 0; jj < depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int base = ii*x*y+kk;
                buffer[base+(y-halo_depth+jj)*x] 
                    = buffer[base+(y-halo_depth-1-jj)*x];
            }
        }
    }
}

// Updates bottom halo.
void update_bottom(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        const int depth, 
        double* buffer)
{
#pragma omp parallel for
    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = 0; jj < depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int base = ii*x*y+kk;
                buffer[base+(halo_depth-jj-1)*x] 
                    = buffer[base+(halo_depth+jj)*x];
            }
        }
    }
}

// Updates front halo.
void update_front(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        const int depth, 
        double* buffer)
{
#pragma omp parallel for collapse(2)
    for(int ii = 0; ii < depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int base = jj*x+kk;
                buffer[base+(z-halo_depth+ii)*x*y] 
                    = buffer[base+(z-halo_depth-1-ii)*x*y];
            }
        }
    }	
}

// Updates back halo.
void update_back(
        const int x,
        const int y,
        const int z,
        const int halo_depth,
        const int depth, 
        double* buffer)
{
#pragma omp parallel for collapse(2)
    for(int ii = 0; ii < depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int base = jj*x+kk;
                buffer[base+(halo_depth-ii-1)*x*y] 
                    = buffer[base+(halo_depth+ii)*x*y];
            }
        }
    }
}


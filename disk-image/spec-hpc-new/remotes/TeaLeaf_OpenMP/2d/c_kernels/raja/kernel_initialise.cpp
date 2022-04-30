#include <stdlib.h>
#include "raja_shared.hpp"
#include "../../settings.h"
#include "../../shared.h"

// Allocates, and zeroes an individual RAJA buffer
void allocate_raja_buffer(
        RAJA::MemoryPool<double>& mem_pool, double** a, int x, int y)
{
    double* temp = mem_pool.allocate(x*y);

    RAJA::RangeSegment seg(0, x*y);
    RAJA::IndexSet is;
    is.push_back(seg);

    RAJA::forall<policy>(is, [=] RAJA_DEVICE (int index)
    {
        temp[index] = 0.0;
    });

    *a = temp;
}

// Allocates, and zeroes an individual buffer
void allocate_buffer(double** a, int x, int y)
{
    *a = (double*)malloc(sizeof(double)*x*y);

    if(*a == NULL) 
    {
        die(__LINE__, __FILE__, "Error allocating buffer %s\n");
    }

#pragma omp parallel for
    for(int jj = 0; jj < y; ++jj)
    {
        for(int kk = 0; kk < x; ++kk)
        {
            const int index = kk + jj*x;
            (*a)[index] = 0.0;
        }
    }
}

// Allocates all of the field buffers
void kernel_initialise(
        Settings* settings, RAJALists** raja_lists, RAJA::MemoryPool<double>& mem_pool, 
        int x, int y, double** density0, double** density, double** energy0, 
        double** energy, double** u, double** u0, double** p, double** r, 
        double** mi, double** w, double** kx, double** ky, double** sd, 
        double** volume, double** x_area, double** y_area, double** cell_x, 
        double** cell_y, double** cell_dx, double** cell_dy, double** vertex_dx, 
        double** vertex_dy, double** vertex_x, double** vertex_y, 
        double** cg_alphas, double** cg_betas, double** cheby_alphas, 
        double** cheby_betas)
{
    print_and_log(settings,
            "Performing this solve with the RAJA %s solver\n",
            settings->solver_name);

    allocate_raja_buffer(mem_pool, density0, x, y);
    allocate_raja_buffer(mem_pool, density, x, y);
    allocate_raja_buffer(mem_pool, energy0, x, y);
    allocate_raja_buffer(mem_pool, energy, x, y);
    allocate_raja_buffer(mem_pool, u, x, y);
    allocate_raja_buffer(mem_pool, u0, x, y);
    allocate_raja_buffer(mem_pool, p, x, y);
    allocate_raja_buffer(mem_pool, r, x, y);
    allocate_raja_buffer(mem_pool, mi, x, y);
    allocate_raja_buffer(mem_pool, w, x, y);
    allocate_raja_buffer(mem_pool, kx, x, y);
    allocate_raja_buffer(mem_pool, ky, x, y);
    allocate_raja_buffer(mem_pool, sd, x, y);
    allocate_raja_buffer(mem_pool, volume, x, y);
    allocate_raja_buffer(mem_pool, x_area, x+1, y);
    allocate_raja_buffer(mem_pool, y_area, x, y+1);
    allocate_raja_buffer(mem_pool, cell_x, x, 1);
    allocate_raja_buffer(mem_pool, cell_y, 1, y);
    allocate_raja_buffer(mem_pool, cell_dx, x, 1);
    allocate_raja_buffer(mem_pool, cell_dy, 1, y);
    allocate_raja_buffer(mem_pool, vertex_dx, x+1, 1);
    allocate_raja_buffer(mem_pool, vertex_dy, 1, y+1);
    allocate_raja_buffer(mem_pool, vertex_x, x+1, 1);
    allocate_raja_buffer(mem_pool, vertex_y, 1, y+1);
    allocate_buffer(cg_alphas, settings->max_iters, 1);
    allocate_buffer(cg_betas, settings->max_iters, 1);
    allocate_buffer(cheby_alphas, settings->max_iters, 1);
    allocate_buffer(cheby_betas, settings->max_iters, 1);

    *raja_lists = new RAJALists(x, y, settings->halo_depth);
}

void kernel_finalise(
        RAJA::MemoryPool<double>& mem_pool, double* density0, double* density, 
        double* energy0, double* energy, double* u, double* u0, double* p, 
        double* r, double* mi, double* w, double* kx, double* ky, double* sd, 
        double* volume, double* x_area, double* y_area, double* cell_x, 
        double* cell_y, double* cell_dx, double* cell_dy, double* vertex_dx, 
        double* vertex_dy, double* vertex_x, double* vertex_y, 
        double* cg_alphas, double* cg_betas, double* cheby_alphas, 
        double* cheby_betas, RAJALists* raja_lists)
{
    mem_pool.release(&density0);
    mem_pool.release(&density);
    mem_pool.release(&energy0);
    mem_pool.release(&energy);
    mem_pool.release(&u);
    mem_pool.release(&u0);
    mem_pool.release(&p);
    mem_pool.release(&r);
    mem_pool.release(&mi);
    mem_pool.release(&w);
    mem_pool.release(&kx);
    mem_pool.release(&ky);
    mem_pool.release(&sd);
    mem_pool.release(&volume);
    mem_pool.release(&x_area);
    mem_pool.release(&y_area);
    mem_pool.release(&cell_x);
    mem_pool.release(&cell_y);
    mem_pool.release(&cell_dx);
    mem_pool.release(&cell_dy);
    mem_pool.release(&vertex_dx);
    mem_pool.release(&vertex_dy);
    mem_pool.release(&vertex_x);
    mem_pool.release(&vertex_y);
    free(cg_alphas);
    free(cg_betas);
    free(cheby_alphas);
    free(cheby_betas);
    delete raja_lists;
}

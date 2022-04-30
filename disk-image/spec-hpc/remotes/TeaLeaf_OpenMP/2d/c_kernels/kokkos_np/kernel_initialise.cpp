#include <stdlib.h>
#include "kokkos_shared.hpp"
#include "../../settings.h"
#include "../../shared.h"

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
        Settings* settings, int x, int y, KView* density0, 
        KView* density, KView* energy0, KView* energy, KView* u, 
        KView* u0, KView* p, KView* r, KView* mi, 
        KView* w, KView* kx, KView* ky, KView* sd, 
        KView* volume, KView* x_area, KView* y_area, KView* cell_x, 
        KView* cell_y, KView* cell_dx, KView* cell_dy, KView* vertex_dx, 
        KView* vertex_dy, KView* vertex_x, KView* vertex_y, KView* comms_buffer,
        Kokkos::View<double*>::HostMirror* host_comms_mirror, 
        double** cg_alphas, double** cg_betas, double** cheby_alphas, 
        double** cheby_betas)
{
    print_and_log(settings,
            "Performing this solve with the Kokkos Nested Parallelism %s solver\n",
            settings->solver_name);

    Kokkos::initialize();

    new(density0) KView("density0", x*y);
    new(density) KView("density", x*y);
    new(energy0) KView("energy0", x*y);
    new(energy) KView("energy", x*y);
    new(u) KView("u", x*y);
    new(u0) KView("u0", x*y);
    new(p) KView("p", x*y);
    new(r) KView("r", x*y);
    new(mi) KView("mi", x*y);
    new(w) KView("w", x*y);
    new(kx) KView("kx", x*y);
    new(ky) KView("ky", x*y);
    new(sd) KView("sd", x*y);
    new(volume) KView("volume", x*y);
    new(x_area) KView("x_area", (x+1)*y);
    new(y_area) KView("y_area", x*(y+1));
    new(cell_x) KView("cell_x", x);
    new(cell_y) KView("cell_y", y);
    new(cell_dx) KView("cell_dx", x);
    new(cell_dy) KView("cell_dy", y);
    new(vertex_dx) KView("vertex_dx", (x+1));
    new(vertex_dy) KView("vertex_dy", (y+1));
    new(vertex_x) KView("vertex_x", (x+1));
    new(vertex_y) KView("vertex_y", (y+1));

    new(comms_buffer) KView("comms_buffer", MAX(x, y)*settings->halo_depth);
    new(host_comms_mirror) KView::HostMirror(); 
    *host_comms_mirror = Kokkos::create_mirror_view(*comms_buffer);

    allocate_buffer(cg_alphas, settings->max_iters, 1);
    allocate_buffer(cg_betas, settings->max_iters, 1);
    allocate_buffer(cheby_alphas, settings->max_iters, 1);
    allocate_buffer(cheby_betas, settings->max_iters, 1);
}

void kernel_finalise(
        double* cg_alphas, double* cg_betas, double* cheby_alphas,
        double* cheby_betas)
{
    free(cg_alphas);
    free(cg_betas);
    free(cheby_alphas);
    free(cheby_betas);

    // TODO: Actually shouldn't be called on a per chunk basis, only by rank
    Kokkos::finalize();
}


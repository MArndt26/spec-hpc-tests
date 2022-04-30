#include "../../drivers/drivers.h"
#include "../../application.h"
#include "../../comms.h"

void solve(chunk_t* chunks, settings_t* settings, int tt, double* wallclock_prev);

// An implementation specific overload of the main timestep loop
void diffuse_overload(chunk_t* chunks, settings_t* settings)
{
    int n = chunks->x*chunks->y*chunks->z;

    print_and_log(settings,
            "This implementation overloads the diffuse function.\n");

    // Currently have to place all structure enclose pointers 
    // into local variables for OMP 4.0 to accept them in mapping clauses
    double* vec_r = chunks->vec_r;
    double* vec_sd = chunks->vec_sd;
    double* vec_kx = chunks->vec_kx;
    double* vec_ky = chunks->vec_ky;
    double* vec_kz = chunks->vec_kz;
    double* vec_z = chunks->vec_z;
    double* vec_w = chunks->vec_w;
    double* vec_p = chunks->vec_p;
    double* cheby_alphas = chunks->cheby_alphas;
    double* cheby_betas = chunks->cheby_betas;
    double* cg_alphas = chunks->cg_alphas;
    double* cg_betas = chunks->cg_betas;
    double* energy = chunks->energy;
    double* density = chunks->density;
    double* energy0 = chunks->energy0;
    double* density0 = chunks->density0;
    double* vec_u = chunks->vec_u;
    double* vec_u0 = chunks->vec_u0;

    settings->is_offload = true;

#pragma omp target data \
    map(to: vec_r[:n], vec_sd[:n], vec_kx[:n], vec_ky[:n], vec_kz[:n], \
            vec_z[:n], vec_w[:n], vec_p[:n], cheby_alphas[:settings->max_iters], \
            cheby_betas[:settings->max_iters], cg_alphas[:settings->max_iters], \
            cg_betas[:settings->max_iters]) \
    map(tofrom: density[:n], energy[:n], density0[:n], energy0[:n], \
            vec_u[:n], vec_u0[:n])
    {
        double wallclock_prev = 0.0;
        for(int tt = 0; tt < settings->end_step; ++tt)
        {
            solve(chunks, settings, tt, &wallclock_prev);
        }
    }

    settings->is_offload = false;

    field_summary_driver(chunks, settings, true);
}


#include "../../kernel_interface.h"
#include "c_kernels.h"

// Initialisation kernels
void run_set_chunk_data(Chunk* chunk, Settings* settings)
{
    set_chunk_data(settings, chunk->x, chunk->y, chunk->z, chunk->left, 
            chunk->bottom, chunk->back, chunk->cell_x, chunk->cell_y, 
            chunk->cell_z, chunk->vertex_x, chunk->vertex_y, chunk->vertex_z,
            chunk->volume, chunk->x_area, chunk->y_area, chunk->z_area);
}

void run_set_chunk_state(Chunk* chunk, Settings* settings, State* states)
{
    set_chunk_state(chunk->x, chunk->y, chunk->z, chunk->vertex_x, 
            chunk->vertex_y, chunk->vertex_z, chunk->cell_x, chunk->cell_y,
            chunk->cell_z, chunk->density, chunk->energy0, chunk->vec_u, 
            settings->num_states, states);
}

void run_kernel_initialise(Chunk* chunk, Settings* settings)
{
    kernel_initialise(settings, chunk->x, chunk->y, chunk->z, &(chunk->density0), 
            &(chunk->density), &(chunk->energy0), &(chunk->energy), 
            &(chunk->vec_u), &(chunk->vec_u0), &(chunk->vec_p), &(chunk->vec_r), 
            &(chunk->vec_mi), &(chunk->vec_w), &(chunk->vec_kx), &(chunk->vec_ky), 
            &(chunk->vec_kz), &(chunk->vec_sd), &(chunk->vec_z), &(chunk->volume), 
            &(chunk->x_area), &(chunk->y_area), &(chunk->z_area), 
            &(chunk->cell_x), &(chunk->cell_y), &(chunk->cell_z), 
            &(chunk->cell_dx), &(chunk->cell_dy), &(chunk->cell_dz), 
            &(chunk->vertex_dx), &(chunk->vertex_dy), &(chunk->vertex_dz), 
            &(chunk->vertex_x), &(chunk->vertex_y), &(chunk->vertex_z), 
            &(chunk->cg_alphas), &(chunk->cg_betas), &(chunk->cheby_alphas),
            &(chunk->cheby_betas), &(chunk->ext->a_row_index), &(chunk->ext->a_col_index),
            &(chunk->ext->a_non_zeros));
}

// Solver-wide kernels
void run_local_halos(
        Chunk* chunk, Settings* settings, int depth)
{
    START_PROFILING(settings->kernel_profile);
    local_halos(chunk->x, chunk->y, chunk->z, depth, settings->halo_depth, 
            chunk->neighbours, settings->fields_to_exchange, chunk->density,
            chunk->energy0, chunk->energy, chunk->vec_u, chunk->vec_p, 
            chunk->vec_sd);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

void run_pack_or_unpack(
        Chunk* chunk, Settings* settings, int depth,
        int face, bool pack, double* field, double* buffer)
{
    START_PROFILING(settings->kernel_profile);
    pack_or_unpack(chunk->x, chunk->y, chunk->z, depth, 
            settings->halo_depth, face, pack, field, buffer);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

void run_store_energy(Chunk* chunk, Settings* settings)
{
    START_PROFILING(settings->kernel_profile);
    store_energy(chunk->x, chunk->y, chunk->z, chunk->energy0, chunk->energy);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

void run_field_summary(
        Chunk* chunk, Settings* settings, 
        double* vol, double* mass, double* ie, double* temp)
{
    START_PROFILING(settings->kernel_profile);
    field_summary(chunk->x, chunk->y, chunk->z,
                    settings->halo_depth, chunk->volume, chunk->density,
                    chunk->energy0, chunk->vec_u, vol, mass, ie, temp);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

// CG solver kernels
void run_cg_solver_init(
        Chunk* chunk, Settings* settings, 
        double rx, double ry, double rz, double* rro)
{
    START_PROFILING(settings->kernel_profile);
    cg_solver_init(chunk->x, chunk->y, chunk->z, 
            settings->halo_depth, settings->coefficient, rx, ry, rz, 
            rro, chunk->density, chunk->energy, chunk->vec_u, 
            chunk->vec_p, chunk->vec_r, chunk->vec_w, 
            chunk->vec_kx, chunk->vec_ky, chunk->vec_kz, 
            chunk->ext->a_row_index, chunk->ext->a_col_index, chunk->ext->a_non_zeros);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

void run_cg_solver_calc_w(Chunk* chunk, Settings* settings, double* pw)
{
    START_PROFILING(settings->kernel_profile);
    cg_solver_calc_w(chunk->x, chunk->y, chunk->z, settings->halo_depth, pw, 
            chunk->vec_p, chunk->vec_w, chunk->vec_kx, chunk->vec_ky, 
            chunk->vec_kz, chunk->ext->a_row_index, chunk->ext->a_col_index, 
            chunk->ext->a_non_zeros);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

void run_cg_solver_calc_ur(
        Chunk* chunk, Settings* settings, double alpha, double* rrn)
{
    START_PROFILING(settings->kernel_profile);
    cg_solver_calc_ur(chunk->x, chunk->y, chunk->z, 
            settings->halo_depth, alpha, rrn, chunk->vec_u, 
            chunk->vec_p, chunk->vec_r, chunk->vec_w);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

void run_cg_solver_calc_p(Chunk* chunk, Settings* settings, double beta)
{
    START_PROFILING(settings->kernel_profile);
    cg_solver_calc_p(chunk->x, chunk->y, chunk->z, 
                    settings->halo_depth, beta, chunk->vec_p, 
                    chunk->vec_r);
    STOP_PROFILING(settings->kernel_profile, __func__);
}


// Chebyshev solver kernels
void run_cheby_solver_init(Chunk* chunk, Settings* settings)
{
    START_PROFILING(settings->kernel_profile);
    cheby_solver_init(
            chunk->x, chunk->y, chunk->z, settings->halo_depth, 
            chunk->theta, chunk->vec_u, chunk->vec_u0, chunk->vec_p, 
            chunk->vec_r, chunk->vec_w, chunk->vec_kx, chunk->vec_ky, 
            chunk->vec_kz, chunk->ext->a_row_index, chunk->ext->a_col_index,
            chunk->ext->a_non_zeros);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

void run_cheby_solver_iterate(
        Chunk* chunk, Settings* settings, double alpha, double beta)
{
    START_PROFILING(settings->kernel_profile);
    cheby_solver_iterate(
            chunk->x, chunk->y, chunk->z, settings->halo_depth, alpha, beta, 
            chunk->vec_u, chunk->vec_u0, chunk->vec_p, chunk->vec_r, chunk->vec_w, 
            chunk->vec_kx, chunk->vec_ky, chunk->vec_kz, chunk->ext->a_row_index, 
            chunk->ext->a_col_index, chunk->ext->a_non_zeros); 
    STOP_PROFILING(settings->kernel_profile, __func__);
}

// Jacobi solver kernels
void run_jacobi_solver_init(
        Chunk* chunk, Settings* settings, double rx, double ry, double rz)
{
    START_PROFILING(settings->kernel_profile);
    jacobi_solver_init(chunk->x, chunk->y, chunk->z,
            settings->halo_depth, settings->coefficient, rx, ry, rz,
            chunk->density, chunk->energy, chunk->vec_u0,
            chunk->vec_u, chunk->vec_kx, chunk->vec_ky,
            chunk->vec_kz);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

void run_jacobi_solver_iterate(
        Chunk* chunk, Settings* settings, double* error)
{
    START_PROFILING(settings->kernel_profile);
    jacobi_solver_iterate(
            chunk->x, chunk->y, chunk->z,
            settings->halo_depth, error, chunk->vec_kx, 
            chunk->vec_ky, chunk->vec_kz, chunk->vec_u0, 
            chunk->vec_u, chunk->vec_r);
    STOP_PROFILING(settings->kernel_profile, __func__);
}


// PPCG solver kernels
void run_ppcg_init(Chunk* chunk, Settings* settings)
{
    START_PROFILING(settings->kernel_profile);
    ppcg_init(chunk->x, chunk->y, chunk->z,
            settings->halo_depth, chunk->theta, chunk->vec_r,
            chunk->vec_sd);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

void run_ppcg_inner_iteration(
        Chunk* chunk, Settings* settings, double alpha, double beta)
{
    START_PROFILING(settings->kernel_profile);
    ppcg_inner_iteration(
            chunk->x, chunk->y, chunk->z, settings->halo_depth, alpha, beta, 
            chunk->vec_u, chunk->vec_r, chunk->vec_kx, chunk->vec_ky,
            chunk->vec_kz, chunk->vec_sd);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

// Shared solver kernels
void run_solver_copy_u(Chunk* chunk, Settings* settings)
{
    START_PROFILING(settings->kernel_profile);
    solver_copy_u(chunk->x, chunk->y, chunk->z, 
            settings->halo_depth, chunk->vec_u0, chunk->vec_u);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

void run_calculate_residual(Chunk* chunk, Settings* settings)
{
    START_PROFILING(settings->kernel_profile);
    calculate_residual(chunk->x, chunk->y, chunk->z, 
            settings->halo_depth, chunk->vec_u, chunk->vec_u0, 
            chunk->vec_r, chunk->vec_kx, chunk->vec_ky, 
            chunk->vec_kz);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

void run_calculate_2norm(
        Chunk* chunk, Settings* settings, double* buffer, double* norm)
{
    START_PROFILING(settings->kernel_profile);
    calculate_2norm(
            chunk->x, chunk->y, chunk->z, 
            settings->halo_depth, buffer, norm);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

void run_solver_finalise(Chunk* chunk, Settings* settings)
{
    START_PROFILING(settings->kernel_profile);
    solver_finalise(chunk->x, chunk->y, chunk->z, settings->halo_depth,
            chunk->energy, chunk->density, chunk->vec_u);
    STOP_PROFILING(settings->kernel_profile, __func__);
}


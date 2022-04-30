#include "../../kernel_interface.h"
#include "c_kernels.h"
#include "cudaknl_shared.h"

#define PRE_KERNEL(pad) \
    const int x_inner = chunk->x - 2*pad; \
    const int y_inner = chunk->y - 2*pad; \
    const int z_inner = chunk->z - 2*pad; \
    const int num_threads = x_inner*y_inner*z_inner; \
    const int num_blocks = ceil((double)num_threads/BLOCK_SIZE); \
    START_PROFILING(settings->kernel_profile);

#define POST_KERNEL(kernel_name) \
    STOP_PROFILING(settings->kernel_profile, kernel_name); \
    check_errors(__LINE__, __FILE__);

void run_set_chunk_data(Chunk* chunk, Settings* settings)
{
    int num_threads = 1 + max(chunk->x, max(chunk->y, chunk->z));
    int num_blocks = ceil((double)num_threads / (double)BLOCK_SIZE);

    START_PROFILING(settings->kernel_profile);

    double x_min = settings->grid_x_min + settings->dx*(double)chunk->left;
    double y_min = settings->grid_y_min + settings->dy*(double)chunk->bottom;
    double z_min = settings->grid_z_min + settings->dz*(double)chunk->back;

    set_chunk_data_vertices<<<num_blocks, BLOCK_SIZE>>>(
            chunk->x, chunk->y, chunk->z, settings->dx,
            settings->dy, settings->dz, x_min, y_min, z_min, 
            chunk->vertex_x, chunk->vertex_y, chunk->vertex_z);

    num_threads = (chunk->x+1)*(chunk->y+1)*(chunk->z+1);
    num_blocks = ceil((double)num_threads / (double)BLOCK_SIZE);

    set_chunk_data<<<num_blocks, BLOCK_SIZE>>>(
            chunk->x, chunk->y, chunk->z, settings->dx,
            settings->dy, settings->dz, chunk->cell_x, chunk->cell_y,
            chunk->cell_z, chunk->vertex_x, chunk->vertex_y, chunk->vertex_z,
            chunk->volume, chunk->x_area, chunk->y_area, chunk->z_area);

    STOP_PROFILING(settings->kernel_profile, "set_chunk_data");
}

void run_set_chunk_state(Chunk* chunk, Settings* settings, State* states)
{
    PRE_KERNEL(0);

    set_chunk_initial_state<<<num_blocks, BLOCK_SIZE>>>(
            chunk->x, chunk->y, chunk->z, states[0].energy, 
            states[0].density, chunk->energy0, chunk->density);

    set_chunk_state<<<num_blocks, BLOCK_SIZE>>>(
            chunk->x, chunk->y, chunk->z, chunk->vertex_x,
            chunk->vertex_y, chunk->vertex_z, chunk->cell_x, chunk->cell_y,
            chunk->cell_z, chunk->density, chunk->energy0, chunk->vec_u,
            state[ii]);

    POST_KERNEL("set_chunk_state");
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
            &(chunk->cheby_betas));
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
    PRE_KERNEL(0);

    store_energy<<<num_blocks, BLOCK_SIZE>>>(
            x_inner, y_inner, z_inner, chunk->energy0, chunk->energy);

    POST_KERNEL("store_energy");
}

void run_field_summary(
        Chunk* chunk, Settings* settings, 
        double* vol, double* mass, double* ie, double* temp)
{
    PRE_KERNEL(settings->halo_depth*2);

    field_summary<<<num_blocks, BLOCK_SIZE>>>(
            x_inner, y_inner, z_inner, settings->halo_depth, 
            chunk->volume, chunk->density, chunk->energy0, 
            chunk->vec_u, vol, mass, ie, temp);

    POST_KERNEL("field_summary");

    sum_reduce_buffer(
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
            chunk->vec_kx, chunk->vec_ky, chunk->vec_kz);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

void run_cg_solver_calc_w(Chunk* chunk, Settings* settings, double* pw)
{
    START_PROFILING(settings->kernel_profile);
    cg_solver_calc_w(chunk->x, chunk->y, chunk->z, 
            settings->halo_depth, pw, chunk->vec_p, 
            chunk->vec_w, chunk->vec_kx,
            chunk->vec_ky, chunk->vec_kz);
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
            chunk->theta, chunk->vec_u, chunk->vec_u0, 
            chunk->vec_p, chunk->vec_r, chunk->vec_w, 
            chunk->vec_kx, chunk->vec_ky, chunk->vec_kz);
    STOP_PROFILING(settings->kernel_profile, __func__);
}

void run_cheby_solver_iterate(
        Chunk* chunk, Settings* settings, double alpha, double beta)
{
    START_PROFILING(settings->kernel_profile);
    cheby_solver_iterate(
            chunk->x, chunk->y, chunk->z, settings->halo_depth, alpha, beta, 
            chunk->vec_u, chunk->vec_u0, chunk->vec_p, chunk->vec_r, chunk->vec_w, 
            chunk->vec_kx, chunk->vec_ky, chunk->vec_kz); 
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


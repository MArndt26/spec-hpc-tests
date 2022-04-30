#include "settings.h"
#include "chunk.h"

/*
 *      This is the main interface file for C based implementations.
 */

// Initialisation kernels
void run_set_chunk_data(
        Chunk* chunk, Settings* settings);
void run_set_chunk_state(
        Chunk* chunk, Settings* settings, State* states);
void run_kernel_initialise(
        Chunk* chunk, Settings* settings);

// Solver-wide kernels
void run_local_halos(
        Chunk* chunk, Settings* settings, int depth);
void run_pack_or_unpack(
        Chunk* chunk, Settings* settings, int depth, int face, bool pack, 
        double* field, double* buffer);
void run_store_energy(
        Chunk* chunk, Settings* settings);
void run_field_summary(
        Chunk* chunk, Settings* settings, 
        double* vol, double* mass, double* ie, double* temp);

// CG solver kernels
void run_cg_solver_init(
        Chunk* chunk, Settings* settings, 
        double rx, double ry, double rz, double* rro);
void run_cg_solver_calc_w(
        Chunk* chunk, Settings* settings, double* pw);
void run_cg_solver_calc_ur(
        Chunk* chunk, Settings* settings, double alpha, double* rrn);
void run_cg_solver_calc_p(
        Chunk* chunk, Settings* settings, double beta);

// Chebyshev solver kernels
void run_cheby_solver_init(
        Chunk* chunk, Settings* settings);
void run_cheby_solver_iterate(
        Chunk* chunk, Settings* settings, double alpha, double beta);

// Jacobi solver kernels
void run_jacobi_solver_init(
        Chunk* chunk, Settings* settings, double rx, double ry, double rz);
void run_jacobi_solver_iterate(
        Chunk* chunk, Settings* settings, double* error);

// PPCG solver kernels
void run_ppcg_init(
        Chunk* chunk, Settings* settings);
void run_ppcg_inner_iteration(
        Chunk* chunk, Settings* settings, double alpha, double beta);

// Shared solver kernels
void run_solver_copy_u(
        Chunk* chunk, Settings* settings);
void run_calculate_residual(
        Chunk* chunk, Settings* settings);
void run_calculate_2norm(
        Chunk* chunk, Settings* settings, double* buffer, double* norm);
void run_solver_finalise(
        Chunk* chunk, Settings* settings);


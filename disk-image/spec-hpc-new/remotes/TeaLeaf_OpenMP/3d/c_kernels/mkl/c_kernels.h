#include "../../settings.h"

/*
 *      This is the main interface file for C based implementations.
 */

// Initialisation kernels
void set_chunk_data(
        Settings* settings, int x, int y, int z, int left,
        int bottom, int back, double* cell_x, double* cell_y, double* cell_z,
		double* vertex_x, double* vertex_y, double* vertex_z, double* volume,
		double* x_area, double* y_area, double* z_area);

void set_chunk_state(
        int x, int y, int z, double* vertex_x, double* vertex_y,
        double* vertex_z, double* cell_x, double* cell_y, double* cell_z,
        double* density, double* energy0, double* vec_u, const int num_states,
        State* state);

void kernel_initialise(
        Settings* settings, int x, int y, int z, double** density0, 
        double** density, double** energy0, double** energy, double** vec_u, 
        double** vec_u0, double** vec_p, double** vec_r, double** vec_mi, 
        double** vec_w, double** vec_kx, double** vec_ky, double** vec_kz, 
        double** vec_sd, double** vec_z, double** volume, double** x_area, 
        double** y_area, double** z_area, double** cell_x, double** cell_y, 
        double** cell_z, double** cell_dx, double** cell_dy, double** cell_dz, 
        double** vertex_dx, double** vertex_dy, double** vertex_dz, 
        double** vertex_x, double** vertex_y, double** vertex_z,
        double** cg_alphas, double** cg_betas, double** cheby_alphas,
        double** cheby_betas, int** a_row_index, int** a_col_index,
        double** a_non_zeros);

// Solver-wide kernels
void local_halos(
        const int x, const int y, const int z, const int depth,
        const int halo_depth, const int* chunk_neighbours,
        const bool* fields_to_exchange, double* density, double* energy0,
        double* energy, double* vec_u, double* vec_p, double* vec_sd);

void pack_or_unpack(
        const int x, const int y, const int z, const int depth,
        const int halo_depth, const int face, bool pack, 
        double *field, double* buffer);

void store_energy(
        int x, int y, int z, double* energy0, double* energy);

void field_summary(
        const int x, const int y, const int z, const int halo_depth,
        double* volume, double* density, double* energy0, double* vec_u,
        double* volOut, double* massOut, double* ieOut, double* tempOut);

// CG solver kernels
void cg_solver_init(
        const int x, const int y, const int z, const int halo_depth,
        const int coefficient, double rx, double ry, double rz, double* rro,
        double* density, double* energy, double* vec_u, double* vec_p, 
        double* vec_r, double* vec_w, double* vec_kx, double* vec_ky, 
        double* vec_kz, int* a_row_index, int* a_col_index, 
        double* a_non_zeros);

void cg_solver_calc_w(
        const int x, const int y, const int z, const int halo_depth, double* pw,
        double* vec_p, double* vec_w, double* vec_kx, double* vec_ky, 
        double* vec_kz, int* a_row_index, int* a_col_index, double* a_non_zeros);

void cg_solver_calc_ur(
        const int x, const int y, const int z, const int halo_depth,
        const double alpha, double* rrn, double* vec_u, double* vec_p,
        double* vec_r, double* vec_w);

void cg_solver_calc_p(
        const int x, const int y, const int z, const int halo_depth,
        const double beta, double* vec_p, double* vec_r);

// Chebyshev solver kernels
void cheby_solver_init(const int x, const int y, const int z,
        const int halo_depth, const double theta, double* vec_u, double* vec_u0,
        double* vec_p, double* vec_r, double* vec_w, double* vec_kx,
        double* vec_ky, double* vec_kz, int* a_row_index, int* a_col_index,
        double* a_non_zeros);

void cheby_solver_iterate(const int x, const int y, const int z,
        const int halo_depth, double alpha, double beta, double* vec_u,
        double* vec_u0, double* vec_p, double* vec_r, double* vec_w,
        double* vec_kx, double* vec_ky, double* vec_kz, int* a_row_index,
        int* a_col_index, double* a_non_zeros); 

// Jacobi solver kernels
void jacobi_solver_init(const int x, const int y, const int z,
        const int halo_depth, const int coefficient, double rx, double ry,
        double rz, double* density, double* energy, double* vec_u0, double* vec_u,
        double* vec_kx, double* vec_ky, double* vec_kz);
void jacobi_solver_iterate(const int x, const int y, const int z,
        const int halo_depth, double* error, double* vec_kx, double* vec_ky,
        double* vec_kz, double* vec_u0, double* vec_u, double* vec_r);

// PPCG solver kernels
void ppcg_init(const int x, const int y, const int z, const int halo_depth,
        double theta, double* vec_r, double* vec_sd);
void ppcg_inner_iteration(const int x, const int y, const int z,
        const int halo_depth, double alpha, double beta, double* vec_u,
        double* vec_r, double* vec_kx, double* vec_ky, double* vec_kz,
        double* vec_sd);

// Shared solver kernels
void solver_copy_u(
        const int x, const int y, const int z, const int halo_depth, 
        double* vec_u0, double* vec_u);

void calculate_residual(
        const int x, const int y, const int z, const int halo_depth,
        double* vec_u, double* vec_u0, double* vec_r, double* vec_kx, 
        double* vec_ky, double* vec_kz);

void calculate_2norm(
        const int x, const int y, const int z, const int halo_depth,
        double* buffer, double* norm);

void solver_finalise(
        const int x, const int y, const int z, const int halo_depth,
        double* energy, double* density, double* u);


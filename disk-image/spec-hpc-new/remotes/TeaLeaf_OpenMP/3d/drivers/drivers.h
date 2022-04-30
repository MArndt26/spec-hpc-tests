#pragma once

#include "../chunk.h"

// Initialisation drivers
void set_chunk_data_driver(
        Chunk* chunk, Settings* settings);
void set_chunk_state_driver(
        Chunk* chunk, Settings* settings, State* states);
void kernel_initialise_driver(
        Chunk* chunks, Settings* settings);

// Halo drivers
void halo_update_driver(
        Chunk* chunks, Settings* settings, int depth);
void remote_halo_driver(
        Chunk* chunks, Settings* settings, int depth);

// Conjugate Gradient solver drivers
void cg_solver_driver(
        Chunk* chunks, Settings* settings, 
        double rx, double ry, double rz, double* error);
void cg_solver_init_driver(
        Chunk* chunks, Settings* settings, 
        double rx, double ry, double rz, double* rro);
void cg_solver_main_step_driver(
        Chunk* chunks, Settings* settings, int tt, 
        double* rro, double* error);

// Chebyshev solver drivers
void cheby_solver_driver(
        Chunk* chunks, Settings* settings, 
        double rx, double ry, double rz, double* error);
void cheby_solver_init_driver(
        Chunk* chunks, Settings* settings, int num_cg_iters, double* bb);
void cheby_solver_coef_driver(
        Chunk* chunks, Settings* settings, int max_iters);
void cheby_solver_main_step_driver(
        Chunk* chunks, Settings* settings, int cheby_iters, 
        bool is_calc_2norm, double* error);

// PPCG solver drivers
void ppcg_solver_driver(
        Chunk* chunks, Settings* settings,
        double rx, double ry, double rz, double* error);
void ppcg_solver_init_driver(
        Chunk* chunks, Settings* settings, double* rro);
void ppcg_solver_main_step_driver(
        Chunk* chunks, Settings* settings, double* rro, double* error);

// Jacobi solver drivers
void jacobi_solver_driver(
        Chunk* chunks, Settings* settings, 
        double rx, double ry, double rz, double* error);
void jacobi_solver_init_driver(
        Chunk* chunks, Settings* settings, 
        double rx, double ry, double rz);
void jacobi_solver_main_step_driver(
        Chunk* chunks, Settings* settings, int tt, double* error);

// Misc drivers
void field_summary_driver(
        Chunk* chunks, Settings* settings, bool solve_finished);
void store_energy_driver(
        Chunk* chunk, Settings* settings);
void solve_finished_driver(
        Chunk* chunks, Settings* settings);
void eigenvalue_driver_initialise(
        Chunk* chunks, Settings* settings, int num_cg_iters);


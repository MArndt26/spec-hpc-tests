#include "../../settings.h"

/* STORE ENERGY */

// Copies energy0 into energy1.
void store_energy(
        const int x, const int y, KView energy, KView energy0);

/* SET CHUNK DATA */

// Initialises the vertices
void set_chunk_data_vertices(
			const int x, const int y, const int halo_depth, KView vertex_x, 
            KView vertex_y, const double x_min, const double y_min, 
            const double dx, const double dy);

// Sets all of the cell data for a chunk
void set_chunk_data(
        const int x, const int y, const int halo_depth, KView vertex_x, 
        KView vertex_y, KView cell_x, KView cell_y, KView volume, 
        KView x_area, KView y_area, const double x_min, const double y_min, 
        const double dx, const double dy);

/* SET CHUNK STATE */

// Sets the initial state for the chunk
void set_chunk_initial_state(
            const int x, const int y, double default_energy, double default_density, 
            KView energy0, KView density);

// Sets all of the additional states in order
void set_chunk_state(
			const int x, const int y, const int halo_depth, State state, 
            KView energy0, KView density, KView u, KView cell_x, KView cell_y, 
            KView vertex_x, KView vertex_y);

/* SOLVER METHODS */

// Copies the inner u into u0.
void copy_u(
        const int x, const int y, const int halo_depth, KView u, KView u0);

// Calculates the residual r.
void calculate_residual(
            const int x, const int y, const int halo_depth, KView u, 
            KView u0, KView r, KView kx, KView ky);

// Calculates the 2 norm of the provided buffer.
void calculate_2norm(
        const int x, const int y, const int halo_depth, KView buffer,
        double* norm); 

// Finalises the energy field.
void finalise(
        const int x, const int y, const int halo_depth, KView u, 
        KView density, KView energy);

/* PPCG SOLVER */

// Initialises Sd
void ppcg_init(
        const int x, const int y, const int halo_depth, const double theta, 
        KView sd, KView r);

// Calculates U and R
void ppcg_calc_ur(
        const int x, const int y, const int halo_depth, KView sd, 
        KView r, KView u, KView kx, KView ky);

// Calculates Sd
void ppcg_calc_sd(
        const int x, const int y, const int halo_depth, const double theta, 
        const double alpha, const double beta, KView sd, KView r);

/* CHEBY SOLVER */

// Initialises the Chebyshev solver
void cheby_init(
        const int x, const int y, const int halo_depth, const double theta, 
        KView p, KView r, KView u, KView u0, KView w, 
        KView kx, KView ky);

// Calculates U
void cheby_calc_u(
        const int x, const int y, const int halo_depth, KView p, 
        KView u);

// The main Cheby iteration step
void cheby_iterate(
        const int x, const int y, const int halo_depth, 
        const double alpha, const double beta, KView p, 
        KView r, KView u, KView u0, KView w, KView kx, 
        KView ky);

/* CG SOLVER */

// Calculates a value for p
void cg_calc_p(
        const int x, const int y, const int halo_depth, const double beta, 
        KView p, KView r);

// Calculates the value of u and r
void cg_calc_ur(
        const int x, const int y, const int halo_depth, KView u, 
        KView r, KView p, KView w, const double alpha, double* rrn);

// Calculates the value for w
void cg_calc_w(
        const int x, const int y, const int halo_depth, KView w, 
        KView p, KView kx, KView ky, double* pw);

// Initialises kx,ky
void cg_init_k(
        const int x, const int y, const int halo_depth, KView w, 
        KView kx, KView ky, const double rx, const double ry);

// Initialises w,r,p and calculates rro
void cg_init_others(
        const int x, const int y, const int halo_depth, KView kx, 
        KView ky, KView p, KView r, KView u, KView w, double* rro);

// Initialises p,r,u,w
void cg_init_u(
            const int x, const int y, const int coefficient, 
            KView p, KView r, KView u, KView w, KView density, 
            KView energy);

/* JACOBI SOLVER */

// Initialises the Jacobi solver
void jacobi_init(
			const int x, const int y, const int halo_depth, 
            const int coefficient, const double rx, const double ry, KView u, 
            KView u0, KView density, KView energy, KView kx, KView ky);

// Copies u into r
void jacobi_copy_u(
        const int x, const int y, KView r, KView u);

// Main Jacobi solver method.
void jacobi_iterate(
        const int x, const int y, const int halo_depth, KView u, 
        KView u0, KView r, KView kx, KView ky, double* error);

/* UPDATING LOCAL HALOS */

// Updates the local left halo region(s)
void update_left(
        const int x, const int y, const int halo_depth, KView buffer, 
        const int face, const int depth); 

// Updates the local right halo region(s)
void update_right(
        const int x, const int y, const int halo_depth, KView buffer, 
        const int face, const int depth);

// Updates the local top halo region(s)
void update_top(
        const int x, const int y, const int halo_depth, KView buffer, 
        const int face, const int depth);

// Updates the local bottom halo region(s)
void update_bottom(
        const int x, const int y, const int halo_depth, KView buffer, 
        const int face, const int depth);

/* PACK HALOS */

// Packs the top halo buffer(s)
void pack_top(
        const int x, const int y, const int halo_depth, 
        KView buffer, KView field, const int depth);

// Packs the bottom halo buffer(s)
void pack_bottom(
        const int x, const int y, const int halo_depth, 
        KView buffer, KView field, const int depth);

// Unpacks the top halo buffer(s)
void unpack_top(
        const int x, const int y, const int halo_depth, 
        KView buffer, KView field, const int depth);

// Unpacks the bottom halo buffer(s)
void unpack_bottom(
        const int x, const int y, const int halo_depth, 
        KView buffer, KView field, const int depth);

// Packs the left halo buffer(s)
void pack_left(
        const int x, const int y, const int halo_depth, 
        KView buffer, KView field, const int depth);

// Packs the right halo buffer(s)
void pack_right(
        const int x, const int y, const int halo_depth, 
        KView buffer, KView field, const int depth);

// Unpacks the left halo buffer(s)
void unpack_left(
        const int x, const int y, const int halo_depth, 
        KView buffer, KView field, const int depth);

// Unpacks the right halo buffer(s)
void unpack_right(
        const int x, const int y, const int halo_depth, 
        KView buffer, KView field, const int depth);


#include <stdlib.h>
#include <math.h>
#include "raja_shared.hpp"
#include "../../shared.h"

/*
 *		JACOBI SOLVER KERNEL
 */

using namespace RAJA;

// Initialises the Jacobi solver
void jacobi_init(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        const int coefficient,
        double rx,
        double ry,
        double* density,
        double* energy,
        double* u0,
        double* u,
        double* kx,
        double* ky)
{
    if(coefficient < CONDUCTIVITY && coefficient < RECIP_CONDUCTIVITY)
    {
        die(__LINE__, __FILE__, "Coefficient %d is not valid.\n", coefficient);
    }

	forall<policy>(
            raja_lists->full_range, [=] RAJA_DEVICE (int index) 
    {
		u0[index] = energy[index]*density[index];
		u[index] = u0[index]; 
	});

	forall<policy>(
            raja_lists->skew_inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
		double densityCentre = (coefficient == CONDUCTIVITY) 
			? density[index] : 1.0/density[index];
		double densityLeft = (coefficient == CONDUCTIVITY) 
			? density[index-1] : 1.0/density[index-1];
		double densityDown = (coefficient == CONDUCTIVITY) 
			? density[index-x] : 1.0/density[index-x];

		kx[index] = rx*(densityLeft+densityCentre)/(2.0*densityLeft*densityCentre);
		ky[index] = ry*(densityDown+densityCentre)/(2.0*densityDown*densityCentre);
	});
}

// The main Jacobi solve step
void jacobi_iterate(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        double* error,
        double* kx,
        double* ky,
        double* u0,
        double* u,
        double* r)
{
    forall<policy>(
            raja_lists->full_range, [=] RAJA_DEVICE (int index) 
    {
		r[index] = u[index];	
	});

    ReduceSum<reduce_policy, double> error_reduce(*error);
    forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
		u[index] = (u0[index] 
			+ (kx[index+1]*r[index+1] + kx[index]*r[index-1])
			+ (ky[index+x]*r[index+x] + ky[index]*r[index-x]))
			/ (1.0 + (kx[index]+kx[index+1])
			+ (ky[index]+ky[index+x]));

		error_reduce += fabs(u[index]-r[index]);
	});

    *error = error_reduce;
}


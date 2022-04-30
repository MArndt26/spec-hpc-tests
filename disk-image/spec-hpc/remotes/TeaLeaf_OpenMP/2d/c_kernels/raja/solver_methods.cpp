#include <stdlib.h>
#include "raja_shared.hpp"
#include "../../shared.h"

using namespace RAJA;

// Copies the current u into u0
void copy_u(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        double* u0,
        double* u)
{
	forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
		u0[index] = u[index];
	});
}

// Calculates the current value of r
void calculate_residual(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        double* u,
        double* u0,
        double* r,
        double* kx,
        double* ky)
{
    forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
		const double smvp = SMVP(u);
		r[index] = u0[index] - smvp;
	});
}

// Calculates the 2 norm of a given buffer
void calculate_2norm(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        double* buffer,
        double* norm)
{
    ReduceSum<reduce_policy, double> norm_reduce(*norm);
	forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
		norm_reduce += buffer[index]*buffer[index];			
	});

    *norm = norm_reduce;
}

// Finalises the solution
void finalise(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        double* energy,
        double* density,
        double* u)
{
	forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
		energy[index] = u[index]/density[index];
	});
}

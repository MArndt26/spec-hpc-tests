#include "raja_shared.hpp"
#include "../../shared.h"

/*
 *		CHEBYSHEV SOLVER KERNEL
 */

using namespace RAJA;

// Calculates the new value for u.
void cheby_calc_u(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        double* u,
        double* p)
{
    forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE(int index) 
    {
		u[index] += p[index];
	});
}

// Initialises the Chebyshev solver
void cheby_init(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        const double theta,
        double* u,
        double* u0,
        double* p,
        double* r,
        double* w,
        double* kx,
        double* ky)
{
	forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
		const double smvp = SMVP(u);
		w[index] = smvp;
		r[index] = u0[index]-w[index];
		p[index] = r[index]/theta;
	});

    cheby_calc_u(raja_lists, x, y, halo_depth, u, p);
}

// The main chebyshev iteration
void cheby_iterate(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        double alpha,
        double beta,
        double* u,
        double* u0,
        double* p,
        double* r,
        double* w,
        double* kx,
        double* ky)
{
	forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
		const double smvp = SMVP(u);
		w[index] = smvp;
		r[index] = u0[index]-w[index];
		p[index] = alpha*p[index] + beta*r[index];
	});

    cheby_calc_u(raja_lists, x, y, halo_depth, u, p);
}


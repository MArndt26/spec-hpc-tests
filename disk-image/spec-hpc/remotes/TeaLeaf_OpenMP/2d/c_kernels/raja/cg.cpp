#include <stdlib.h>
#include "raja_shared.hpp"
#include "../../shared.h"

/*
 *		CONJUGATE GRADIENT SOLVER KERNEL
 */

using namespace RAJA;

// Initialises the CG solver
void cg_init(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        const int coefficient,
        double rx,
        double ry,
        double* rro,
        double* density,
        double* energy,
        double* u,
        double* p,
        double* r,
        double* w,
        double* kx,
        double* ky)
{
    if(coefficient != CONDUCTIVITY && coefficient != RECIP_CONDUCTIVITY)
    {
        die(__LINE__, __FILE__, "Coefficient %d is not valid.\n", coefficient);
    }

	forall<policy>(
            raja_lists->full_range, [=] RAJA_DEVICE (int index) 
    {
        p[index] = 0.0;
        r[index] = 0.0;
        u[index] = energy[index]*density[index];
    });

	forall<policy>(
            raja_lists->in_one_domain_list, [=] RAJA_DEVICE (int index) 
    {
        w[index] = (coefficient == CONDUCTIVITY) 
            ? density[index] : 1.0/density[index];
    });

    forall<policy>(
            raja_lists->skew_inner_domain_list, [=] RAJA_DEVICE(int index) 
    {
        kx[index] = rx*(w[index-1]+w[index]) /
            (2.0*w[index-1]*w[index]);
        ky[index] = ry*(w[index-x]+w[index]) /
            (2.0*w[index-x]*w[index]);
    });

    ReduceSum<reduce_policy, double> rro_reduce(0.0);
    forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
	    const double smvp = SMVP(u);
	    w[index] = smvp;
	    r[index] = u[index]-w[index];
	    p[index] = r[index];

	    rro_reduce += r[index]*p[index];
	});

    *rro += rro_reduce;
}

// Calculates w
void cg_calc_w(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        double* pw,
        double* p,
        double* w,
        double* kx,
        double* ky)
{
    ReduceSum<reduce_policy, double> pw_reduce(0.0);
	forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
        w[index] = SMVP(p);	    
        pw_reduce += w[index]*p[index];
	});

    *pw += pw_reduce;
}

// Calculates u and r
void cg_calc_ur(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        const double alpha,
        double* rrn,
        double* u,
        double* p,
        double* r,
        double* w)
{
    ReduceSum<reduce_policy, double> rrn_reduce(0.0);
    forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
        u[index] += alpha*p[index];
        r[index] -= alpha*w[index];
        rrn_reduce += r[index]*r[index];
    });

    *rrn += rrn_reduce;
}

// Calculates p
void cg_calc_p(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        const double beta,
        double* p,
        double* r)
{
    forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
        p[index] = beta*p[index] + r[index];
    });
}


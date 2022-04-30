#include "raja_shared.hpp"
#include "../../shared.h"

using namespace RAJA;

// Initialises the PPCG solver
void ppcg_init(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        double theta,
        double* r,
        double* sd)
{
	forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
		sd[index] = r[index]/theta;
	});
}

// The PPCG inner iteration
void ppcg_inner_iteration(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        double alpha,
        double beta,
        double* u,
        double* r,
        double* kx,
        double* ky,
        double* sd)
{
    forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
		const double smvp = SMVP(sd);
		r[index] -= smvp;
		u[index] += sd[index];
	});

    forall<policy>(
            raja_lists->inner_domain_list, [=] RAJA_DEVICE (int index) 
    {
		sd[index] = alpha*sd[index]+beta*r[index];
	});
}


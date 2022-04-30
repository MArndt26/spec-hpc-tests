#include "raja_shared.hpp"
#include "../../shared.h"

using namespace RAJA;

// Store original energy state
void store_energy(
        RAJALists* raja_lists,
        double* energy0,
        double* energy)
{
	forall<policy>(raja_lists->full_range, [=] RAJA_DEVICE (int index) 
    {
		energy[index] = energy0[index];
	});
}


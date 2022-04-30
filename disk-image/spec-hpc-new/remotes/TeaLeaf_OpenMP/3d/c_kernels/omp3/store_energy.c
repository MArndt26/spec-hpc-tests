#include "../../shared.h"

// Store original energy state
void store_energy(
        int x,
        int y,
        int z,
        double* energy0,
        double* energy)
{
#pragma omp parallel for
    for(int ii = 0; ii < x*y*z; ++ii)
    {
        energy[ii] = energy0[ii];
    }
}


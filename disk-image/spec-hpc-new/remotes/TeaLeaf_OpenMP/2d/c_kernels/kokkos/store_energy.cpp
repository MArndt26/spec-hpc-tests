#include "kokkos_shared.hpp"

using namespace Kokkos;

// Copies energy0 into energy1.
void store_energy(
        const int x, const int y, KView energy, KView energy0) 
{
    parallel_for(x*y, KOKKOS_LAMBDA (const int index)
    {
		energy(index) = energy0(index);
	});
}


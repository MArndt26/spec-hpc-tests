#pragma once
#include "kokkos_shared.hpp"

// Copies energy0 into energy1.
template <class Device>
struct StoreEnergy
{
	typedef Device device_type;

	StoreEnergy(KView energy, KView energy0) 
		: energy(energy), energy0(energy0) { }

	KOKKOS_INLINE_FUNCTION
	void operator()(const int index) const 
    {
		energy(index) = energy0(index);
	}

	KView energy;
	KView energy0;
};


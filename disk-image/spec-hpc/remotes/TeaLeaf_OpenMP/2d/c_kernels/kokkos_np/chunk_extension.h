#pragma once
#include "kokkos_shared.hpp"

typedef Kokkos::View<double*, DEVICE> FieldBufferType;

// Empty extension point
typedef struct ChunkExtension
{
    FieldBufferType comms_buffer;
    Kokkos::View<double*>::HostMirror host_comms_mirror; 

} ChunkExtension;


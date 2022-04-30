#include "kokkos_shared.hpp"
#include "../../shared.h"

using namespace Kokkos;

// Updates the local left halo region(s)
void update_left(
        const int x, const int y, const int halo_depth, KView buffer, 
        const int face, const int depth) 
{
    parallel_for(y*depth, KOKKOS_LAMBDA (const int index)
    {
        const size_t flip = index % depth;
        const size_t lines = index/depth;
        const size_t offset = lines*(x - depth);
        const size_t to_index = offset + halo_depth - depth + index;
        const size_t from_index = to_index + 2*(depth - flip) - 1;
        buffer(to_index) = buffer(from_index);
    });
}

// Updates the local right halo region(s)
void update_right(
        const int x, const int y, const int halo_depth, KView buffer, 
        const int face, const int depth) 
{
    parallel_for(y*depth, KOKKOS_LAMBDA (const int index)
    {
        const size_t flip = index % depth;
        const size_t lines = index/depth;
        const size_t offset = x-halo_depth + lines*(x-depth);
        const size_t to_index = offset+index;
        const size_t from_index = to_index-(1+flip*2);
        buffer(to_index) = buffer(from_index);
    });
}

// Updates the local top halo region(s)
void update_top(
        const int x, const int y, const int halo_depth, KView buffer, 
        const int face, const int depth) 
{
    parallel_for(x*depth, KOKKOS_LAMBDA (const int index)
    {
         const size_t lines = index/x;
         const size_t offset = x*(y-halo_depth);

         const size_t to_index = offset+index;
         const size_t from_index = to_index-(1+lines*2)*x;
         buffer(to_index) = buffer(from_index);
    });
}

// Updates the local bottom halo region(s)
void update_bottom(
        const int x, const int y, const int halo_depth, KView buffer, 
        const int face, const int depth) 
{
    parallel_for(x*depth, KOKKOS_LAMBDA (const int index)
    {
         const size_t lines = index/x;
         const size_t offset = x*halo_depth;

         const size_t from_index = offset+index;
         const size_t to_index = from_index-(1+lines*2)*x;
         buffer(to_index) = buffer(from_index);
    });
}


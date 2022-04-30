#pragma once
#include "kokkos_shared.hpp"
#include "../../shared.h"

// Updates an individual buffer halo.
template <class Device>
struct LocalHalos
{
	typedef Device device_type;

	LocalHalos(const int x, const int y, const int halo_depth, KView buffer, 
            const int face, const int depth) 
		: x(x), y(y), halo_depth(halo_depth), buffer(buffer), face(face), 
        depth(depth) {}

	KOKKOS_INLINE_FUNCTION
	void operator()(const int index) const 
    {
		switch(face)
		{
			case CHUNK_LEFT:
				{
					int flip = index % depth;
					int lines = index/depth;
					int offset = lines*(x - depth);
          int to_index = offset + halo_depth - depth + index;
          int from_index = to_index + 2*(depth - flip) - 1;
					buffer(to_index) = buffer(from_index);
					break;
				}
			case CHUNK_RIGHT:
				{
					int flip = index % depth;
					int lines = index/depth;
					int offset = x-halo_depth + lines*(x-depth);
					int to_index = offset+index;
					int from_index = to_index-(1+flip*2);
					buffer(to_index) = buffer(from_index);
					break;
				}
			case CHUNK_TOP:
				{
					int lines = index/x;
					int offset = x*(y-halo_depth);
					int to_index = offset+index;
					int from_index = to_index-(1+lines*2)*x;
					buffer(to_index) = buffer(from_index);
					break;
				}
			case CHUNK_BOTTOM:
				{
					int lines = index/x;
					int offset = x*halo_depth;
					int from_index = offset+index;
					int to_index = from_index-(1+lines*2)*x;
					buffer(to_index) = buffer(from_index);
					break;
				}
			default:
				break;
		}
	}

    const int x;
    const int y;
	const int face;
	const int depth;
    const int halo_depth;
	KView buffer;
};


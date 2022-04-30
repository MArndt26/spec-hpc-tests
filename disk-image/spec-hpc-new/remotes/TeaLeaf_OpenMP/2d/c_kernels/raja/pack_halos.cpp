#include <stdlib.h>
#include "raja_shared.hpp"
#include "../../shared.h"

using namespace RAJA;

// Either packs or unpacks data from/to buffers.
void pack_or_unpack(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int depth,
        const int halo_depth,
        const int face,
        bool pack,
        double *field,
        double* buffer)
{
	//ListISet list = (pack) 
    //    ? raja_lists->pack_list[face][depth-1] 
	//	: raja_lists->unpack_list[face][depth-1];

    //forall_packing<policy>(list, [&](int index, int buffer_offset) 
    //{
	//	if(pack) 
	//	{
	//		buffer[buffer_offset] = field[index];
	//	}
	//	else 
	//	{
	//		field[index] = buffer[buffer_offset];
	//	}
    //});
}


#include <stdlib.h>
#include "../../shared.h"
#include "pack_halos.cuknl"

typedef void (*pack_kernel_f)( 
        const int, const int, const int, const int, 
        double*, double*, const int);

// Either packs or unpacks data from/to buffers.
void pack_or_unpack(
        Chunk* chunk, Settings* settings, int depth, int face, 
        bool pack, double* field, double* buffer)
{
    pack_kernel_f kernel = NULL;

    const int x_inner = x - 2*halo_depth;
    const int y_inner = y - 2*halo_depth;
    const int z_inner = z - 2*halo_depth;

    int buffer_length = 0;

    switch(face)
    {
        case CHUNK_LEFT:
            kernel = pack ? pack_left : unpack_left;
            buffer_length = y_inner*z_inner*depth;
            break;
        case CHUNK_RIGHT:
            kernel = pack ? pack_right : unpack_right;
            buffer_length = y_inner*z_inner*depth;
            break;
        case CHUNK_TOP:
            kernel = pack ? pack_top : unpack_top;
            buffer_length = x_inner*z_inner*depth;
            break;
        case CHUNK_BOTTOM:
            kernel = pack ? pack_bottom : unpack_bottom;
            buffer_length = x_inner*z_inner*depth;
            break;
        case CHUNK_FRONT:
            kernel = pack ? pack_front : unpack_front;
            buffer_length = x_inner*y_inner*depth;
            break;
        case CHUNK_BACK:
            kernel = pack ? pack_back : unpack_back;
            buffer_length = x_inner*y_inner*depth;
            break;
        default:
            die(__LINE__, __FILE__, "Incorrect face provided: %d.\n", face);
    }

    if(!pack)
    {
        cudaMemcpy(
                d_comm_buffer, buffer, buffer_length*sizeof(double), 
                cudaMemcpyHostToDevice);
        check_errors(__LINE__, __FILE__);
    }

    int num_blocks = ceil(buffer_length / (double)BLOCK_SIZE);
    kernel<<<num_blocks, BLOCK_SIZE>>>(
            x, y, z, depth, halo_depth, field, chunk->d_comm_buffer);

    if(pack)
    {
        cudaMemcpy(
                d_comm_buffer, buffer, buffer_length*sizeof(double),
                cudaMemcpyDeviceToHost);
        check_errors(__LINE__, __FILE__);
    }
}


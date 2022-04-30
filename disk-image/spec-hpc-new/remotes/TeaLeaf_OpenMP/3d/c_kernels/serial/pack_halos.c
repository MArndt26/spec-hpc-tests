#include <stdlib.h>
#include "../../shared.h"

// Packs left data into buffer.
void pack_left(
        const int x,
        const int y,
        const int z,
        const int depth,
        const int halo_depth,
        double* field,
        double* buffer)
{
    const int y_inner = y-2*halo_depth;

    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < halo_depth+depth; ++kk)
            {
                int bufIndex = (kk-halo_depth) + (jj-halo_depth)*depth + 
                    (ii-halo_depth)*depth*y_inner;
                buffer[bufIndex] = field[ii*y*x+jj*x+kk];
            }
        }
    }
}

// Packs right data into buffer.
void pack_right(
        const int x,
        const int y,
        const int z,
        const int depth,
        const int halo_depth,
        double* field,
        double* buffer)
{
    const int y_inner = y-2*halo_depth;

    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = x-halo_depth-depth; kk < x-halo_depth; ++kk)
            {
                int bufIndex = (kk-(x-halo_depth-depth)) + (jj-halo_depth)*depth + 
                    (ii-halo_depth)*depth*y_inner;
                buffer[bufIndex] = field[ii*y*x+jj*x+kk];
            }
        }
    }
}

// Packs top data into buffer.
void pack_top(
        const int x,
        const int y,
        const int z,
        const int depth,
        const int halo_depth,
        double* field,
        double* buffer)
{
    const int x_inner = x-2*halo_depth;

    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = y-halo_depth-depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int bufIndex = (kk-halo_depth) + (jj-(y-halo_depth-depth))*x_inner 
                    + (ii-halo_depth)*depth*x_inner;
                buffer[bufIndex] = field[ii*y*x+jj*x+kk];
            }
        }
    }
}

// Packs bottom data into buffer.
void pack_bottom(
        const int x,
        const int y,
        const int z,
        const int depth,
        const int halo_depth,
        double* field,
        double* buffer)
{
    const int x_inner = x-2*halo_depth;

    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < halo_depth+depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int bufIndex = (kk-halo_depth) + (jj-halo_depth)*x_inner + 
                    (ii-halo_depth)*depth*x_inner;
                buffer[bufIndex] = field[ii*y*x+jj*x+kk];
            }
        }
    }
}

// Packs front data into buffer.
void pack_front(
        const int x,
        const int y,
        const int z,
        const int depth,
        const int halo_depth,
        double* field,
        double* buffer)
{
    const int x_inner = x-2*halo_depth;
    const int y_inner = y-2*halo_depth;

    for(int ii = z-halo_depth-depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int bufIndex = (kk-halo_depth) + (jj-halo_depth)*x_inner 
                    + (ii-(z-halo_depth-depth))*x_inner*y_inner;

                buffer[bufIndex] = field[ii*y*x+jj*x+kk];
            }
        }
    }
}

// Packs back data into buffer.
void pack_back(
        const int x,
        const int y,
        const int z,
        const int depth,
        const int halo_depth,
        double* field,
        double* buffer)
{
    const int x_inner = x-2*halo_depth;
    const int y_inner = y-2*halo_depth;

    for(int ii = halo_depth; ii < halo_depth+depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int bufIndex = (kk-halo_depth) + (jj-halo_depth)*x_inner 
                    + (ii-halo_depth)*x_inner*y_inner;
                buffer[bufIndex] = field[ii*y*x+jj*x+kk];
            }
        }
    }
}

// Unpacks left data from buffer.
void unpack_left(
        const int x,
        const int y,
        const int z,
        const int depth,
        const int halo_depth,
        double* field,
        double* buffer)
{
    const int y_inner = y-2*halo_depth;

    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth-depth; kk < halo_depth; ++kk)
            {
                int bufIndex = (kk-(halo_depth-depth)) + (jj-halo_depth)*depth + 
                    (ii-halo_depth)*depth*y_inner;
                field[ii*y*x+jj*x+kk] = buffer[bufIndex];
            }
        }
    }
}

// Unpacks right data from buffer.
void unpack_right(
        const int x,
        const int y,
        const int z,
        const int depth,
        const int halo_depth,
        double* field,
        double* buffer)
{
    const int y_inner = y-2*halo_depth;

    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = x-halo_depth; kk < x-halo_depth+depth; ++kk)
            {
                int bufIndex = (kk-(x-halo_depth)) + (jj-halo_depth)*depth + 
                    (ii-halo_depth)*depth*y_inner;
                field[ii*y*x+jj*x+kk] = buffer[bufIndex];
            }
        }
    }
}

// Unpacks top data from buffer.
void unpack_top(
        const int x,
        const int y,
        const int z,
        const int depth,
        const int halo_depth,
        double* field,
        double* buffer)
{
    const int x_inner = x-2*halo_depth;

    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = y-halo_depth; jj < y-halo_depth+depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int bufIndex = (kk-halo_depth) + (jj-(y-halo_depth))*x_inner + 
                    (ii-halo_depth)*depth*x_inner;
                field[ii*y*x+jj*x+kk] = buffer[bufIndex];
            }
        }
    }
}

// Unpacks bottom data from buffer.
void unpack_bottom(
        const int x,
        const int y,
        const int z,
        const int depth,
        const int halo_depth,
        double* field,
        double* buffer)
{
    const int x_inner = x-2*halo_depth;

    for(int ii = halo_depth; ii < z-halo_depth; ++ii)
    {
        for(int jj = halo_depth-depth; jj < halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int bufIndex = (kk-halo_depth) + (jj-(halo_depth-depth))*x_inner + 
                    (ii-halo_depth)*depth*x_inner;
                field[ii*y*x+jj*x+kk] = buffer[bufIndex];
            }
        }
    }
}

// Unpacks front data from buffer.
void unpack_front(
        const int x,
        const int y,
        const int z,
        const int depth,
        const int halo_depth,
        double* field,
        double* buffer)
{
    const int x_inner = x-2*halo_depth;
    const int y_inner = y-2*halo_depth;

    for(int ii = z-halo_depth; ii < z-halo_depth+depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int bufIndex = (kk-halo_depth) + (jj-halo_depth)*x_inner + 
                    (ii-(z-halo_depth))*x_inner*y_inner;
                field[ii*y*x+jj*x+kk] = buffer[bufIndex];
            }
        }
    }
}

// Unpacks back data from buffer.
void unpack_back(
        const int x,
        const int y,
        const int z,
        const int depth,
        const int halo_depth,
        double* field,
        double* buffer)
{
    const int x_inner = x-2*halo_depth;
    const int y_inner = y-2*halo_depth;

    for(int ii = halo_depth-depth; ii < halo_depth; ++ii)
    {
        for(int jj = halo_depth; jj < y-halo_depth; ++jj)
        {
            for(int kk = halo_depth; kk < x-halo_depth; ++kk)
            {
                int bufIndex = (kk-halo_depth) + (jj-halo_depth)*x_inner + 
                    (ii-(halo_depth-depth))*x_inner*y_inner;
                field[ii*y*x+jj*x+kk] = buffer[bufIndex];
            }
        }
    }
}

typedef void (*pack_kernel_f)(int,int,int,int,int,double*,double*);

// Either packs or unpacks data from/to buffers.
void pack_or_unpack(
        const int x,
        const int y,
        const int z,
        const int depth,
        const int halo_depth,
        const int face,
        bool pack,
        double *field,
        double* buffer)
{
    pack_kernel_f kernel = NULL;

    switch(face)
    {
        case CHUNK_LEFT:
            kernel = pack ? pack_left : unpack_left;
            break;
        case CHUNK_RIGHT:
            kernel = pack ? pack_right : unpack_right;
            break;
        case CHUNK_TOP:
            kernel = pack ? pack_top : unpack_top;
            break;
        case CHUNK_BOTTOM:
            kernel = pack ? pack_bottom : unpack_bottom;
            break;
        case CHUNK_FRONT:
            kernel = pack ? pack_front : unpack_front;
            break;
        case CHUNK_BACK:
            kernel = pack ? pack_back : unpack_back;
            break;
        default:
            die(__LINE__, __FILE__, "Incorrect face provided: %d.\n", face);
    }

    kernel(x, y, z, depth, halo_depth, field, buffer);
}


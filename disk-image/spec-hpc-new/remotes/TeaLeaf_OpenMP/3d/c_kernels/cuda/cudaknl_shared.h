#pragma once

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 256
#endif

__device__ inline double SUM(double a, double b)
{
	return a + b;
}

template < typename T, int offset >
class Reduce
{
	public:
		__device__ inline static void Run(T* array, T* out, T(*func)(T, T))
		{
			//only need to synch if not working within a warp
			if (offset > 16)
			{
				__syncthreads();
			}

			// only continue if it's in the lower half
			if (threadIdx.x < offset)
			{
				array[threadIdx.x] = func(array[threadIdx.x], array[threadIdx.x + offset]);
				Reduce< T, offset/2 >::Run(array, out, func);
			}
		}
};

template < typename T >
class Reduce < T, 0 >
{
	public:
		__device__ inline static void Run(T* array, T* out, T(*func)(T, T))
		{
			out[blockIdx.x] = array[0];
		}
};


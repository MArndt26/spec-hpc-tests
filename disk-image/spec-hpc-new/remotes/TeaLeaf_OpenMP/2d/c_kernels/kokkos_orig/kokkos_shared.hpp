#pragma once 

#include <Kokkos_Core.hpp>
#include <Kokkos_Parallel.hpp>
#include <Kokkos_View.hpp>

#ifdef CUDA
    #define DEVICE Kokkos::Cuda
#endif

#ifdef OPENMP
    #define DEVICE Kokkos::OpenMP
#endif

#ifndef DEVICE
    #define DEVICE Kokkos::OpenMP
#endif

typedef Kokkos::View<double*, DEVICE> KView;

class KokkosHelper
{
	public:
        // Used to manage copying the raw pointers
		template <class Type>
		static void PackMirror(
                const Kokkos::View<Type*,Kokkos::HostSpace>& mirror, 
                const Type* buffer, int len)
		{
			for(int kk = 0; kk != len; ++kk)
			{
				mirror(kk) = buffer[kk];
			}
		}

		template <class Type>
		static void UnpackMirror(
                Type* buffer, 
                const Kokkos::View<Type*,Kokkos::HostSpace>& mirror, 
                int len)
		{
			for(int kk = 0; kk != len; ++kk)
			{
				buffer[kk] = mirror(kk);
			}
		}

        static void GetKokkosArrayVal(
                int len, KView device_buffer, const char* name)
        {
            typename Kokkos::View<double*>::HostMirror host_buffer =
                create_mirror_view(device_buffer);
            Kokkos::deep_copy(host_buffer, device_buffer);

            for(int ii = 0; ii < len; ++ii)
            {
            printf("%.12e\n", host_buffer[ii]);
            }

        }
};


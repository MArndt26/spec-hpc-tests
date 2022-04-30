#pragma once

#if defined(RAJA_USE_CUDA) // CUDA managed memory allocate/release

#include <cuda.h>
#include <cuda_runtime.h>

template <typename T>
inline T *Allocate(size_t size)
{
    T *retVal ;
    gpuErrchk( cudaMallocManaged((void **)&retVal, sizeof(T)*size, cudaMemAttachGlobal) ) ;
    return retVal ;
}

template <typename T>
inline void Release(T **ptr)
{
    if (*ptr != NULL) {
        gpuErrchk( cudaFree(*ptr) ) ;
        *ptr = NULL ;
    }
}

template <typename T>
inline void Release(T * __restrict__ *ptr)
{
    if (*ptr != NULL) {
        gpuErrchk( cudaFree(*ptr) ) ;
        *ptr = NULL ;
    }
}

#else  // Standard CPU memory allocate/release

#include <cstdlib>
#include <cstring>

template <typename T>
inline T *Allocate(size_t size)
{
    T *retVal ;
    posix_memalign((void **)&retVal, RAJA::DATA_ALIGN, sizeof(T)*size);
    return retVal ;
}

template <typename T>
inline void Release(T **ptr)
{
    if (*ptr != NULL) {
        free(*ptr) ;
        *ptr = NULL ;
    }
}

template <typename T>
inline void Release(T * __restrict__ *ptr)
{
    if (*ptr != NULL) {
        free(*ptr) ;
        *ptr = NULL ;
    }
}

#endif 

/**********************************/
/* Memory Pool                    */
/**********************************/

namespace RAJA {

    template <typename VARTYPE >
        struct MemoryPool {
            public:
                MemoryPool()
                {
                    for (int i=0; i<32; ++i) {
                        lenType[i] = 0 ;
                        ptr[i] = 0 ;
                    }
                }

                VARTYPE *allocate(int len) {
                    VARTYPE *retVal ;
                    int i ;
                    for (i=0; i<32; ++i) {
                        if (lenType[i] == len) {
                            lenType[i] = -lenType[i] ;
                            retVal = ptr[i] ;
                            break ;
                        }
                        else if (lenType[i] == 0) {
                            lenType[i] = -len ;
                            ptr[i] = Allocate<VARTYPE>(len) ;
                            retVal = ptr[i] ;
                            break ;
                        }
                    }
                    if (i == 32) {
                        retVal = 0 ;  /* past max available pointers */
                    }
                    return retVal ;
                }

                bool release(VARTYPE **oldPtr) {
                    int i ;
                    bool success = true ;
                    for (i=0; i<32; ++i) {
                        if (ptr[i] == *oldPtr) {
                            lenType[i] = -lenType[i] ;
                            *oldPtr = 0 ;
                            break ;
                        }
                    }
                    if (i == 32) {
                        success = false ; /* error -- not found */
                    }
                    return success ;
                }

                bool release(VARTYPE * __restrict__ *oldPtr) {
                    int i ;
                    bool success = true ;
                    for (i=0; i<32; ++i) {
                        if (ptr[i] == *oldPtr) {
                            lenType[i] = -lenType[i] ;
                            *oldPtr = 0 ;
                            break ;
                        }
                    }
                    if (i == 32) {
                        success = false ; /* error -- not found */
                    }
                    return success ; 
                }

                VARTYPE *ptr[32] ; 
                int lenType[32] ;
        } ;

}


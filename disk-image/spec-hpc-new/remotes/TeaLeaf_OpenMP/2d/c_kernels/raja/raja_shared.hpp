#pragma once 

#include <vector>
#include "RAJA/RAJA.hxx"
#include "mem_pool.hpp"
#include "../../shared.h"

#ifdef RAJA_USE_CUDA
    const size_t block_size = 128;
    typedef RAJA::IndexSet::ExecPolicy<
        RAJA::seq_segit,
        RAJA::cuda_exec<block_size>> policy;

    typedef RAJA::cuda_reduce<block_size> reduce_policy;
#else
    typedef RAJA::IndexSet::ExecPolicy<
        RAJA::seq_segit, 
        RAJA::omp_parallel_for_exec> policy;
    typedef RAJA::omp_reduce reduce_policy;
#endif

/*
 *      RAJA HELPER
 *      Helper class devoted to RAJA.
 */
struct RAJALists
{
    RAJALists(const int xCells, const int yCells, const int halo_depth);
    ~RAJALists();

    RAJA::IndexSet inner_domain_list;
	RAJA::IndexSet in_one_domain_list;
	RAJA::IndexSet skew_inner_domain_list;
	RAJA::IndexSet full_range;

	std::vector<RAJA::IndexSet> pack_list[NUM_FACES];
	std::vector<RAJA::IndexSet> unpack_list[NUM_FACES];
    std::vector<RAJA::IndexSet> lr_halo_list;
    std::vector<RAJA::IndexSet> tb_halo_list;

    const int halo_depth;
};


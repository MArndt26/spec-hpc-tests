#include "raja_shared.hpp"
#include "RAJA/IndexSetBuilders.hxx"

using namespace RAJA;

// Precomputes all of the index lists.
RAJALists::RAJALists(
        const int x,
        const int y,
        const int halo_depth)
: halo_depth(halo_depth)
{
    Index_type innerExtent[2] ;
    Index_type innerStride[2] ;
    int num_indices ;

    Index_type* indices = new Index_type[x*y];
    Index_type* indices2 = new Index_type[x*y];

#if defined(RAJA_BOX_SEGMENT)
    innerExtent[0] = x ;
    innerExtent[1] = y ;
    innerStride[0] = 1 ;
    innerStride[1] = x ;
    BoxSegment range(0, 2, &innerExtent[0], &innerStride[0]) ;
    full_range.push_back(range);
#else
    /* this may be faster */
    RangeSegment range(0, x*y);
    full_range.push_back(range);
#endif

#if defined(RAJA_BOX_SEGMENT)
    innerExtent[0] = x - 2*halo_depth ;
    innerExtent[1] = y - 2*halo_depth ;
    innerStride[0] = 1 ;
    innerStride[1] = x ;
    BoxSegment id_list(halo_depth*x + halo_depth, 2,
                       &innerExtent[0], &innerStride[0]) ;
    inner_domain_list.push_back(id_list);
#else
    num_indices = 0;
    // Inner domain (excluding HALO)
    for(int jj = halo_depth; jj < y-halo_depth; ++jj)
    {
        for(int kk = halo_depth; kk < x-halo_depth; ++kk)
        {
            indices[num_indices++] = jj*x + kk;
        }
    }
    ListSegment id_list(indices, num_indices);
    inner_domain_list.push_back(id_list);
#endif

    // Inner domain with one sided instep
#if defined(RAJA_BOX_SEGMENT)
    innerExtent[0] = x - halo_depth - 1 ;
    innerExtent[1] = y - halo_depth - 1 ;
    innerStride[0] = 1 ;
    innerStride[1] = x ;
    BoxSegment si_list(halo_depth*x + halo_depth, 2,
                       &innerExtent[0], &innerStride[0]) ;
    skew_inner_domain_list.push_back(si_list);
#else
    num_indices = 0;
    for(int jj = halo_depth; jj < y-1; ++jj)
    {
        for(int kk = halo_depth; kk < x-1; ++kk)
        {
            indices[num_indices++] = jj*x + kk;
        }
    }
    ListSegment si_list(indices, num_indices);
    skew_inner_domain_list.push_back(si_list);
#endif

    // Domain in one step
#if defined(RAJA_BOX_SEGMENT)
    innerExtent[0] = x - 2 ;
    innerExtent[1] = y - 2 ;
    innerStride[0] = 1 ;
    innerStride[1] = x ;
    BoxSegment io_list(x + 1, 2,
                       &innerExtent[0], &innerStride[0]) ;
    in_one_domain_list.push_back(io_list);
#else
    num_indices = 0;
    for(int jj = 1; jj < y-1; ++jj)
    {
        for(int kk = 1; kk < x-1; ++kk)
        {
            indices[num_indices++] = jj*x + kk;
        }
    }
    ListSegment io_list(indices, num_indices);
    in_one_domain_list.push_back(io_list);
#endif

    // Left right halo lists
    for(int dd = 1; dd <= halo_depth; ++dd)
    {
        RangeSegment list(0, y*dd);
        IndexSet set;
        set.push_back(list);
        lr_halo_list.push_back(set);
    }

    // Top bottom halo lists
    for(int dd = 1; dd <= halo_depth; ++dd)
    {
        RangeSegment list(0, x*dd);
        IndexSet set;
        set.push_back(list);
        tb_halo_list.push_back(set);
    }

    delete[] indices;
}

RAJALists::~RAJALists()
{
}

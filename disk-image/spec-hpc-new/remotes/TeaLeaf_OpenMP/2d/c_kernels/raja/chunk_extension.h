#pragma once
#include "raja_shared.hpp"

typedef double* FieldBufferType;

// Empty extension point
typedef struct ChunkExtension
{
    RAJALists* raja_lists;
    RAJA::MemoryPool< double > mem_pool;

} ChunkExtension;

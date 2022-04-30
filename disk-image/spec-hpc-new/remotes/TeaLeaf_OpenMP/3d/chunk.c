#include "chunk.h"

// Initialise the chunk
void initialise_chunk(Chunk* chunk, Settings* settings, int x, int y, int z)
{
    // Initialise the key variables
    chunk->x = x + settings->halo_depth*2;
    chunk->y = y + settings->halo_depth*2;
    chunk->z = z + settings->halo_depth*2;
    chunk->dt_init = settings->dt_init;

    // Allocate the neighbour list
    chunk->neighbours = (int*)malloc(sizeof(int)*NUM_FACES);

    chunk->lr_page = (chunk->y)*(chunk->z);
    chunk->fb_page = (chunk->x)*(chunk->y);
    chunk->tb_page = (chunk->x)*(chunk->z);

    // Allocate the MPI comm buffers
    int lr_len = chunk->lr_page*settings->halo_depth*NUM_FIELDS;
    chunk->left_send = (double*)malloc(sizeof(double)*lr_len);
    chunk->left_recv = (double*)malloc(sizeof(double)*lr_len);
    chunk->right_send = (double*)malloc(sizeof(double)*lr_len);
    chunk->right_recv = (double*)malloc(sizeof(double)*lr_len);

    int fb_len = chunk->fb_page*settings->halo_depth*NUM_FIELDS;
    chunk->front_send = (double*)malloc(sizeof(double)*fb_len);
    chunk->front_recv = (double*)malloc(sizeof(double)*fb_len);
    chunk->back_send = (double*)malloc(sizeof(double)*fb_len);
    chunk->back_recv = (double*)malloc(sizeof(double)*fb_len);

    int tb_len = chunk->tb_page*settings->halo_depth*NUM_FIELDS;
    chunk->top_send = (double*)malloc(sizeof(double)*tb_len);
    chunk->top_recv = (double*)malloc(sizeof(double)*tb_len);
    chunk->bottom_send = (double*)malloc(sizeof(double)*tb_len);
    chunk->bottom_recv = (double*)malloc(sizeof(double)*tb_len);

    chunk->ext = (chunk_extension_t*)malloc(sizeof(chunk_extension_t));
}

// Finalise the chunk
void finalise_chunk(Chunk* chunk)
{
    free(chunk->neighbours);
}

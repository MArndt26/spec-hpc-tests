#include "chunk.h"

// Initialise the chunk
void initialise_chunk(Chunk* chunk, Settings* settings, int x, int y)
{
  // Initialise the key variables
  chunk->x = x + settings->halo_depth*2;
  chunk->y = y + settings->halo_depth*2;
  chunk->dt_init = settings->dt_init;

  // Allocate the neighbour list
  chunk->neighbours = (int*)malloc(sizeof(int)*NUM_FACES);

  // Allocate the MPI comm buffers
  int lr_len = chunk->y*settings->halo_depth*NUM_FIELDS;
  chunk->left_send = (double*)malloc(sizeof(double)*lr_len);
  chunk->left_recv = (double*)malloc(sizeof(double)*lr_len);
  chunk->right_send = (double*)malloc(sizeof(double)*lr_len);
  chunk->right_recv = (double*)malloc(sizeof(double)*lr_len);

  int tb_len = chunk->x*settings->halo_depth*NUM_FIELDS;
  chunk->top_send = (double*)malloc(sizeof(double)*tb_len);
  chunk->top_recv = (double*)malloc(sizeof(double)*tb_len);
  chunk->bottom_send = (double*)malloc(sizeof(double)*tb_len);
  chunk->bottom_recv = (double*)malloc(sizeof(double)*tb_len);

  // Initialise the ChunkExtension, which allows composition of extended
  // fields specific to individual implementations
  chunk->ext = (ChunkExtension*)malloc(sizeof(ChunkExtension));
}

// Finalise the chunk
void finalise_chunk(Chunk* chunk)
{
  free(chunk->neighbours);
  free(chunk->ext);
  free(chunk->left_send);
  free(chunk->left_recv);
  free(chunk->right_send);
  free(chunk->right_recv);
  free(chunk->top_send);
  free(chunk->top_recv);
  free(chunk->bottom_send);
  free(chunk->bottom_recv);
}

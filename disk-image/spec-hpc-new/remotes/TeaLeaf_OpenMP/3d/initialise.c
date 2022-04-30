#include <string.h>
#include <float.h>
#include "chunk.h"
#include "settings.h"
#include "application.h"
#include "drivers/drivers.h"

void decompose_field(Settings* settings, Chunk* chunks);

// Initialise settings from input file
void initialise_application(Chunk** chunks, Settings* settings)
{
    State* states = NULL;
    read_config(settings, &states);

    *chunks = (Chunk*)malloc(sizeof(Chunk)*settings->num_chunks_per_rank);

    decompose_field(settings, *chunks);
    kernel_initialise_driver(*chunks, settings);
    set_chunk_data_driver(*chunks, settings);
    set_chunk_state_driver(*chunks, settings, states);

    // Prime the initial halo data
    reset_fields_to_exchange(settings);
    settings->fields_to_exchange[FIELD_DENSITY] = true;
    settings->fields_to_exchange[FIELD_ENERGY0] = true;
    settings->fields_to_exchange[FIELD_ENERGY1] = true;
    halo_update_driver(*chunks, settings, 2);

    store_energy_driver(*chunks, settings);
}

// Decomposes the field into multiple chunks
void decompose_field(Settings* settings, Chunk* chunks)
{
    // Calculates the num chunks field is to be decomposed into
    settings->num_chunks = settings->num_ranks * 
        settings->num_chunks_per_rank;

    int num_chunks = settings->num_chunks;

    double best_metric = DBL_MAX;
    double x_cells = (double)settings->grid_x_cells;
    double y_cells = (double)settings->grid_y_cells;
    double z_cells = (double)settings->grid_z_cells;
    int x_chunks = 0;
    int y_chunks = 0;
    int z_chunks = 0;

    // Decompose by minimal surface area to volume
    for(int xx = 1; xx <= num_chunks; ++xx)
    {
        if(num_chunks % xx) continue;

        // Calculate number of chunks grouped by x split
        int num_chunks_in_wedge = num_chunks / xx;

        for(int yy = 1; yy <= num_chunks_in_wedge; ++yy)
        {
            if(num_chunks_in_wedge % yy) continue;

            int zz = num_chunks / (xx*yy);
            int num_chunks_in_pencil = num_chunks_in_wedge / yy;

            if(num_chunks_in_pencil % zz) continue;

            double surface = ((x_cells/xx)*(x_cells/xx) +
                    (y_cells/yy)*(y_cells/yy) +
                    (z_cells/zz)*(z_cells/zz)) * 2;
            double volume = (x_cells/xx)*(y_cells/yy)*(z_cells/zz);

            double current_metric = surface / volume;

            // Save improved decompositions
            if(current_metric < best_metric)
            {
                x_chunks = xx;
                y_chunks = yy;
                z_chunks = zz;
                best_metric = current_metric;
            }
        }
    }

    // Check that the decomposition didn't fail
    if(!x_chunks || !y_chunks || !z_chunks)
    {
        die(__LINE__, __FILE__, 
                "Failed to decompose the field with given parameters.\n");
    }

    // TODO: CHECK IMPLICIT CAST WORKS HERE
    int dx = settings->grid_x_cells / x_chunks;
    int dy = settings->grid_y_cells / y_chunks;
    int dz = settings->grid_z_cells / z_chunks;

    int mod_x = settings->grid_x_cells % x_chunks;
    int mod_y = settings->grid_y_cells % y_chunks;
    int mod_z = settings->grid_z_cells % z_chunks;
    int add_x_prev = 0;
    int add_y_prev = 0;
    int add_z_prev = 0;

    // Compute the full decomposition on all ranks
    for(int zz = 0; zz < z_chunks; ++zz)
    {
        // Potentially round up chunk size
        int add_z = (zz < mod_z);

        for(int yy = 0; yy < y_chunks; ++yy)
        {
            int add_y = (yy < mod_y);

            for(int xx = 0; xx < x_chunks; ++xx)
            {
                int add_x = (xx < mod_x);
                int chunk = xx + yy*x_chunks + zz*y_chunks*x_chunks;

                for(int cc = 0; cc < settings->num_chunks_per_rank; ++cc)
                {
                    int rank = cc + settings->rank*settings->num_chunks_per_rank;

                    // Store the values for all chunks local to rank
                    if(rank == chunk)
                    {
                        initialise_chunk(&(chunks[cc]), settings, 
                                dx+add_x, dy+add_y, dz+add_z);

                        // Set up the mesh ranges
                        chunks[cc].left = xx*dx + add_x_prev;
                        chunks[cc].right = chunks[cc].left + dx + add_x;
                        chunks[cc].bottom = yy*dy + add_y_prev;
                        chunks[cc].top = chunks[cc].bottom + dy + add_y;
                        chunks[cc].back = zz*dz + add_z_prev;
                        chunks[cc].front = chunks[cc].front + dz + add_z;

                        // Set up the chunk connectivity
                        chunks[cc].neighbours[CHUNK_LEFT] = 
                            (xx == 0) 
                            ? EXTERNAL_FACE 
                            : chunk - 1;
                        chunks[cc].neighbours[CHUNK_RIGHT] = 
                            (xx == x_chunks-1) 
                            ? EXTERNAL_FACE 
                            : chunk + 1;
                        chunks[cc].neighbours[CHUNK_BOTTOM] = 
                            (yy == 0) 
                            ? EXTERNAL_FACE 
                            : chunk - x_chunks;
                        chunks[cc].neighbours[CHUNK_TOP] = 
                            (yy == y_chunks-1) 
                            ? EXTERNAL_FACE 
                            : chunk + x_chunks;
                        chunks[cc].neighbours[CHUNK_BACK] = 
                            (zz == 0) 
                            ? EXTERNAL_FACE 
                            : chunk - x_chunks*y_chunks;
                        chunks[cc].neighbours[CHUNK_FRONT] = 
                            (zz == z_chunks-1) 
                            ? EXTERNAL_FACE 
                            : chunk + x_chunks*y_chunks;
                    }
                }

                // If chunks rounded up, maintain relative location
                add_x_prev += add_x;
            }
            add_x_prev = 0;
            add_y_prev += add_y;
        }
        add_y_prev = 0;
        add_z_prev += add_z;
    }
}

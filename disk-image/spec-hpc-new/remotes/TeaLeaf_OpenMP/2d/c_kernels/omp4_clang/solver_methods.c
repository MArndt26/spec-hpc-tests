#include <stdlib.h>
#include <omp.h>
#include "../../shared.h"

/*
 *		SHARED SOLVER METHODS
 */

// Copies the current u into u0
void copy_u(
    const int x,
    const int y,
    const int halo_depth,
    double* u0,
    double* u)
{
#pragma omp target teams distribute parallel for collapse(2)
  for(int jj = halo_depth; jj < y-halo_depth; ++jj)
  {
    for(int kk = halo_depth; kk < x-halo_depth; ++kk)
    {
      const int index = kk + jj*x;
      u0[index] = u[index];	
    }
  }
}

// Calculates the current value of r
void calculate_residual(
    const int x,
    const int y,
    const int halo_depth,
    double* u,
    double* u0,
    double* r,
    double* kx,
    double* ky)
{
#pragma omp target teams distribute parallel for collapse(2)
  for(int jj = halo_depth; jj < y-halo_depth; ++jj)
  {
    for(int kk = halo_depth; kk < x-halo_depth; ++kk)
    {
      const int index = kk + jj*x;
      const double smvp = SMVP(u);
      r[index] = u0[index] - smvp;
    }
  }
}

// Calculates the 2 norm of a given buffer
void calculate_2norm(
    const int x,
    const int y,
    const int halo_depth,
    double* buffer,
    double* norm)
{
  const int nb = 512;
  double* reduce_temp = (double*)malloc(sizeof(double)*nb);
#pragma omp target teams distribute parallel for collapse(2) map(tofrom: reduce_temp[:nb]) //reduction(+:norm_temp)
  for(int jj = halo_depth; jj < y-halo_depth; ++jj)
  {
    for(int kk = halo_depth; kk < x-halo_depth; ++kk)
    {
      const int index = kk + jj*x;
      reduce_temp[omp_get_team_num()] += buffer[index]*buffer[index];			
    }
  }

  for(int ii = 0; ii < nb; ++ii) {
    *norm += reduce_temp[ii];
  }
}

// Finalises the solution
void finalise(
    const int x,
    const int y,
    const int halo_depth,
    double* energy,
    double* density,
    double* u)
{
#pragma omp target teams distribute parallel for collapse(2)
  for(int jj = halo_depth; jj < y-halo_depth; ++jj)
  {
    for(int kk = halo_depth; kk < x-halo_depth; ++kk)
    {
      const int index = kk + jj*x;
      energy[index] = u[index]/density[index];
    }
  }
}
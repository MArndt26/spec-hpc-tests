#include "raja_shared.hpp"
#include "../../shared.h"

using namespace RAJA;

// The field summary kernel
void field_summary(
        RAJALists* raja_lists,
        const int x,
        const int y,
        const int halo_depth,
        double* volume,
        double* density,
        double* energy0,
        double* u,
        double* vol_out,
        double* mass_out,
        double* ie_out,
        double* temp_out)
{
    double vol_reduce(0.0);
    double ie_reduce(0.0);
    double temp_reduce(0.0);
    double mass_reduce(0.0);

    for(int jj = halo_depth; jj < y-halo_depth; ++jj)
    {
        for(int kk = halo_depth; kk < x-halo_depth; ++kk)
        {
            const int index = kk + jj*x;
            double cell_vol = volume[index];
            double cell_mass = cell_vol*density[index];
            vol_reduce += cell_vol;
            mass_reduce += cell_mass;
            ie_reduce += cell_mass*energy0[index];
            temp_reduce += cell_mass*u[index];
        }
    }

    *vol_out += vol_reduce;
    *ie_out += ie_reduce;
    *temp_out += temp_reduce;
    *mass_out += mass_reduce;
}

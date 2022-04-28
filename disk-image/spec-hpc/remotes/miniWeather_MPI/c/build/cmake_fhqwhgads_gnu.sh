#!/bin/bash

export TEST_MPI_COMMAND="mpirun -n 1"

./cmake_clean.sh

/home/cmake-install/bin/cmake -DCMAKE_CXX_COMPILER=mpic++                   \
      -DPNETCDF_PATH=/home/PnetCDF \
      -DOPENMP_FLAGS="-fopenmp"                     \
      -DCXXFLAGS="-O3 -DNO_INFORM"                                \
      -DLDFLAGS=""                                  \
      -DNX=200 \
      -DNZ=100 \
      -DSIM_TIME=2 \
      -DOUT_FREQ=1000 \
      ..


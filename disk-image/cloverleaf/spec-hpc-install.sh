# install build-essential (gcc and g++ included) and gfortran

#Compile SPEC hpc benchmark

echo "12345" | sudo apt-get install build-essential gfortran
echo "build essential done"

# NOTE: we need to build
# echo "building CloverLeaf OpenMP"
# cd /home/gem5/CloverLeaf_OpenMP
# make COMPILER=GNU MPI_COMPILER=gfortran C_MPI_COMPILER=gcc

echo "Done Building SPEC hpc dependencies"
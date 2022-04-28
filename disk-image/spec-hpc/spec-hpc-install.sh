# install build-essential (gcc and g++ included) and gfortran

#Compile SPEC hpc benchmark

echo "12345" | sudo apt-get install build-essential gfortran
echo "build essential done"

cd /home
wget https://github.com/Kitware/CMake/releases/download/v3.23.1/cmake-3.23.1.tar.gz
tar xzf cmake-3.23.1.tar.gz
cd cmake-3.23.1
./bootstrap --prefix=/home/cmake-install/
make
make install
PATH=/home/cmake-install/bin:$PATH ; export PATH
which cmake
/home/cmake-install/bin/cmake -version
echo "cmake done"

echo "12345" | sudo apt-get install -y m4
echo "m4 done"
echo "12345" | sudo apt-get install libopenmpi-dev  
echo "libopenmpi done"
echo "12345" | sudo apt-get install libpnetcdf-dev
echo "libpnetcdf done"
echo "12345" | sudo apt-get install ncview
echo "ncview done"



echo "building CloverLeaf MPI"
cd /home/gem5/CloverLeaf_MPI/
make COMPILER=GNU

echo "building CloverLeaf OpenMP"
cd /home/gem5/CloverLeaf_OpenMP
make COMPILER=GNU MPI_COMPILER=gfortran C_MPI_COMPILER=gcc

echo "Installing mpich-install script"
cd /home/
wget https://www.mpich.org/static/downloads/3.4.1/mpich-3.4.1.tar.gz
gunzip mpich-3.4.1.tar.gz
tar xf mpich-3.4.1.tar
rm mpich-3.4.1.tar.gz
cd /home/mpich-3.4.1
./configure --prefix=/home/mpich-install 2>&1 --with-device=ch3 | tee c.txt
make 2>&1 | tee m.txt
make install 2>&1 | tee mi.txt
PATH=/home/mpich-install/bin:$PATH ; export PATH
echo "Testing mpicc: "
which mpicc


echo "building miniWeather deps"
cd /home/

wget https://parallel-netcdf.github.io/Release/pnetcdf-1.12.3.tar.gz
gunzip pnetcdf-1.12.3.tar.gz
tar xf pnetcdf-1.12.3.tar
ls -lah
rm pnetcdf-1.12.3.tar.gz
cd /home/pnetcdf-1.12.3 
./configure --prefix=/home/PnetCDF MPICC=/home/mpich-install/bin/mpicc
make -s LIBTOOLFLAGS=--silent V=1 -j8
make install
PATH=/home/PnetCDF/bin:$PATH ; export PATH
echo "testing PnetCDF"
which ncmpidump
which ncmpidiff

echo "building miniWeather"
cd /home/gem5/miniWeather_MPI/c/build
source cmake_fhqwhgads_gnu.sh
make

echo "Done Building SPEC hpc dependencies"
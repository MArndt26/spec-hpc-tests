ifndef CORES
$(error CORES is not set)
endif

export PATH=/home/mpich-install/bin:/home/PnetCDF/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games
export LD_LIBRARY_PATH=/home/mpich-install/lib

all:
	@echo "No Test Selected"

clean: cloverleaf_clean	

cloverleaf_build:
	@echo "Building CloverLeaf"
	cd CloverLeaf_MPI; make COMPILER=GNU

cloverleaf_run:
	@echo "Running CloverLeaf on $(CORES) processes"
	cd CloverLeaf_MPI; mpirun -np $(CORES) ./clover_leaf

cloverleaf_clean: source_path
	@echo "Cleaning CloverLeaf"
	cd CloverLeaf_MPI; make clean


miniweather_build:
	@echo "Building miniWeather"
	cd miniWeather_MPI/c/build; source cmake_fhqwhgads_gnu.sh; make; 

miniweather_run:
	@echo "Running miniWeather on $(CORES) processes"
	cd  miniWeather_MPI/c/build; mpirun -n $(CORES) ./mpi

miniweather_clean:
	@echo "Cleaning miniWeather"
	cd miniWeather_MPI/c/build; source cmake_clean.sh
all: 
	@echo "Error: No option selected!"

cloverleaf_build:
	@echo "Building Cloverleaf"
	cd CloverLeaf_OpenMP/; make COMPILER=GNU MPI_COMPILER=gfortran C_MPI_COMPILER=gcc

cloverleaf_run:
	@echo "Running Cloverleaf"
ifndef CORES
	$(error CORES is not set)
endif
	cd CloverLeaf_OpenMP/; export OMP_NUM_THREADS=$(CORES) ; ./clover_leaf

cloverleaf_clean:
	@echo "Cleaning Cloverleaf"
	cd CloverLeaf_OpenMP; make clean
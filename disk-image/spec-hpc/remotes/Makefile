all: 
	@echo "Error: No option selected!"

cloverleaf_build:
	@echo "Building Cloverleaf"
	cd CloverLeaf_OpenMP/; make COMPILER=GNU MPI_COMPILER=gfortran C_MPI_COMPILER=gcc

tealeaf_build:
	@echo "Building Tealeaf"
	cd TeaLeaf_OpenMP/; make clean; make CC=gcc CPP=g++ KERNELS=omp4

cloverleaf_run:
	@echo "Running Cloverleaf"
ifndef CORES
	$(error CORES is not set)
endif
	cd CloverLeaf_OpenMP/; export OMP_NUM_THREADS=$(CORES) ; ./clover_leaf

tealeaf_run:
	@echo "Running Tealeaf"
ifndef CORES
	$(error CORES is not set)
endif
	cd TeaLeaf_OpenMP/; export OMP_NUM_THREADS=$(CORES) ; sudo ./tealeaf

cloverleaf_clean:
	@echo "Cleaning Cloverleaf"
	cd CloverLeaf_OpenMP; make clean
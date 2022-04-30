RAJA_LIB 	 		 = $(RAJA_PATH)/lib/
RAJA_INC 	 		 = $(RAJA_PATH)/include/

vpath %.cpp $(KERNELS_PATH)
vpath %.hpp $(KERNELS_PATH)

TL_LINK		 = $(CPP)
TL_COMPILER  = $(CPP)
TL_FLAGS 	 = $(CPPFLAGS) -I$(RAJA_INC)
TL_LDFLAGS	+= $(OMP) -lRAJA -L$(RAJA_LIB)

# Currently a very constrained compilation
ifeq ($(CUDA), yes)
  OPTIONS      += -DNO_MPI
  KERNEL_CPP	= nvcc
  KERNEL_FLAGS  = -ccbin g++ -O2 --expt-extended-lambda -restrict \
  				  -arch compute_35 -std=c++11 -Xcompiler -fopenmp --x cu \
  				  $(KERNELS_CPP) -DRAJA_USE_CUDA -I$(RAJA_INC) -I$(KERNELS_PATH)
  TL_LDFLAGS   += -lcudart -fopenmp -L/$(CUDA_PATH)/lib64
else
  KERNEL_CPP	= $(CPP)
  KERNEL_FLAGS  = $(TL_FLAGS) -I$(KERNELS_PATH)
endif

KERNEL_SRCS := $(wildcard $(KERNELS_PATH)/*.cpp)
KERNEL_OBJS := $(patsubst $(KERNELS_PATH)/%.cpp, obj/$(KERNELS)/%.o, $(KERNEL_SRCS))

include $(KERNELS_PATH)/make.deps

build_kernels: $(KERNEL_OBJS) Makefile
	@echo "Built RAJA kernels"

obj/$(KERNELS)/%.o: %.cpp Makefile make.deps $(KERNELS_PATH)/make.deps
	$(KERNEL_CPP) $(KERNEL_FLAGS) $(OPTIONS) -c $< -o $@

obj/$(KERNELS)/%.o: %.cxx Makefile make.deps $(KERNELS_PATH)/make.deps
	$(KERNEL_CPP) $(KERNEL_FLAGS) $(OPTIONS) -c $< -o $@

# Treat the chunk.c as an independent C++ source file for interfacing
obj/$(KERNELS)/chunk.o: chunk.c Makefile make.deps
	$(KERNEL_CPP) $(KERNEL_FLAGS) $(OPTIONS) -c $< -o $@


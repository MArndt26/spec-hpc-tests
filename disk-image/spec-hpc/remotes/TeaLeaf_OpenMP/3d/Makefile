# Used specified parameters
KERNELS  	= omp3
CPROFILER   = yes
DEBUG       = no
OPTIONS     = #-DNO_MPI
CC		    = mpiicc #icc #gcc
COMPILER    = INTEL

include make.flags
LDLIBS 		= #-lrt -lm

# Apply additional options
ifeq ($(CPROFILER), yes)
    OPTIONS += -DENABLE_PROFILING
endif

# Build the nested kernel implementations
KERNELS_PATH = c_kernels/$(KERNELS)

vpath %.c drivers/

# Get all objects
OBJS  = $(patsubst %.c, build/obj/%.o, $(wildcard *.c))
OBJS += $(patsubst drivers/%.c, build/obj/%.o, $(wildcard drivers/*.c))

tealeaf: $(OBJS) build_kernels Makefile 
	$(CC) $(CFLAGS) $(OBJS) $(KERNEL_OBJS) $(LDLIBS) -o tealeaf

include make.deps
include $(KERNELS_PATH)/Makefile

build/obj/%.o: %.c Makefile make.deps $(KERNELS_PATH)/make.deps | make_build_dir
	$(CC) $(CFLAGS) $(OPTIONS) -c $< -o $@

make_build_dir:
	@mkdir -p build
	@mkdir -p build/obj
	-@cp $(KERNELS_PATH)/*.h build/

.PHONY: clean

clean:
	rm -rf build/ tealeaf

## Populates make.deps with dependencies
#build/obj/%.o: %.c Makefile make.deps
#	gcc -MM $< >> make.deps


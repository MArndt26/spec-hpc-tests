vpath %.c $(KERNELS_PATH)

# Overwrite as limited to clang
CC		    = clang #icc #gcc
COMPILER    = CLANG

include $(KERNELS_PATH)/make.deps

KERNEL_SRCS := $(wildcard $(KERNELS_PATH)/*.c)
KERNEL_OBJS := $(patsubst $(KERNELS_PATH)/%.c, obj/%.o, $(KERNEL_SRCS))

build_kernels: $(KERNEL_OBJS)
	@echo "Built OpenMP 4.0 kernels"
	-mv *.ptx *.cub obj/

# Add the extra flags
CFLAGS     	+= -openmp=libomp -DDIFFUSE_OVERLOAD -omptargets=nvptx64sm_35-nvidia-cuda
LDLIBS 		+= -L/usr/lib64/gcc/x86_64-suse-linux/4.7/ -B/usr/lib64/gcc/x86_64-suse-linux/4.7/


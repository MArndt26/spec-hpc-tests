vpath %.c $(KERNELS_PATH)

NV_FLAGS  = -I$(CUDA_PATH)/include $(CODE_GEN_$(NV_ARCH)) -restrict \
		 	-Xcompiler "$(CFLAGS_GNU)" 

ifeq ($(DEBUG),yes)
  NV_FLAGS += -O0 -g -G
else
  NV_FLAGS += -O3 --ptxas-options="-v"
endif

LDFLAGS  += -L$(CUDA_PATH)/lib64

include $(KERNELS_PATH)/make.deps

KERNEL_SRCS := $(wildcard $(KERNELS_PATH)/*.c)
KERNEL_OBJS := $(patsubst $(KERNELS_PATH)/%.c, obj/%.o, $(KERNEL_SRCS))

KERNEL_SRCS += $(wildcard $(KERNELS_PATH)/*.cu)
KERNEL_OBJS += $(patsubst $(KERNELS_PATH)/%.cu, obj/%.o, $(KERNEL_SRCS))

build_kernels: $(KERNEL_OBJS)
	@echo "Built CUDA kernels"

%.o: %.cu Makefile make.deps
	nvcc $(NV_FLAGS) -c $<


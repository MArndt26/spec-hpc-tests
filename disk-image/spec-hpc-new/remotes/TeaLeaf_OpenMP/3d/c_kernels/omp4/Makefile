vpath %.c $(KERNELS_PATH)

include $(KERNELS_PATH)/make.deps

KERNEL_SRCS := $(wildcard $(KERNELS_PATH)/*.c)
KERNEL_OBJS := $(patsubst $(KERNELS_PATH)/%.c, build/obj/%.o, $(KERNEL_SRCS))

build_kernels: $(KERNEL_OBJS)
	@echo "Built OpenMP 4.0 kernels"
	-mv *.ptx *.cub build/obj/

CFLAGS += -DDIFFUSE_OVERLOAD


vpath %.c $(KERNELS_PATH)

include $(KERNELS_PATH)/make.deps

KERNEL_SRCS := $(wildcard $(KERNELS_PATH)/*.c)
KERNEL_OBJS := $(patsubst $(KERNELS_PATH)/%.c, build/obj/%.o, $(KERNEL_SRCS))

build_kernels: $(KERNEL_OBJS)
	@echo "Built MKL kernels"

CFLAGS += -I${MKLROOT}/include

## MKL 11.3
#LDLIBS += -L${MKLROOT}/lib/intel64 -lmkl_intel_lp64 -lmkl_core \
#		  -lmkl_intel_thread -lmkl_blacs_intelmpi_lp64 -lpthread -lm

## MKL 11.2
#LDLIBS += -L${MKLROOT}/lib/intel64 -lmkl_scalapack_lp64 -lmkl_intel_lp64 \
#		  -lmkl_core -lmkl_intel_thread -lmkl_blacs_intelmpi_lp64 -lpthread -lm

# MKL 11.2 Static linking
LDLIBS += -Wl,--start-group \
		  ${MKLROOT}/lib/intel64/libmkl_intel_lp64.a \
		  ${MKLROOT}/lib/intel64/libmkl_core.a \
		  ${MKLROOT}/lib/intel64/libmkl_intel_thread.a \
		  ${MKLROOT}/lib/intel64/libmkl_blacs_lp64.a \
		  -Wl,--end-group -lpthread -lm

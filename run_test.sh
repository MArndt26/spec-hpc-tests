#!/bin/bash

# run system configuration
./gem5/build/X86/gem5.opt x86-spec-hpc-benchmarks.py --benchmark tealeaf --cores 16

# view run output
# cat gem5/m5out/board.pc.com_1.device | less +G


# for b in {"tealeaf"} ;
# do
#     for cpu in {1, 2, 4, 8, 16} ;
#     do
#         for s in {"base"} ;
#         do
#             echo "Starting run with benchmark: $b, cpu: $cpu, size: $s"
#             ( ./my_script & )
#         ; done
#     ; done
# ; done
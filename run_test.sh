#!/bin/bash

# run system configuration

# view run output
# cat gem5/m5out/board.pc.com_1.device | less +G

b=05.lbm
s=t

for cpu in 16 8 4 2 1
do
    echo "Starting run with benchmark: $b, cpu: $cpu, size: $s"
    ./gem5/build/X86/gem5.opt x86-spec-hpc-benchmarks.py --benchmark $b --size $s --cores 16
done
#!/bin/bash

# run system configuration
./gem5/build/X86/gem5.opt x86-spec-hpc-benchmarks.py --benchmark tealeaf

# view run output
# cat gem5/m5out/board.pc.com_1.device | less +G
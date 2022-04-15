#!/bin/bash

# run system configuration
./gem5/build/X86/gem5.opt spec-hpc-run.py

# view run output
cat gem5/m5out/board.pc.com_1.device | less +G
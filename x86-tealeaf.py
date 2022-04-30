# Copyright (c) 2021 The Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# This run script is based off the gem5/configs/example/gem5_library/x86-npb-benchmarks.py
# script.  We have used this as a template and modified it to run the benchmarks
# for SPEChpc 2021 benchmarks.

"""
Script to run SPEC hpc 2021 benchmarks with gem5. The script expects the
benchmark program to run. The input is in the format
<benchmark_prog>.<class>.x .The system is fixed with 8 CPU cores, MESI
Two Level system cache and 3 GB DDR4 memory. It uses the x86 board.

This script will count the total number of instructions executed
in the ROI. It also tracks how much wallclock and simulated time.

Usage:
------

```
./gem5/build/X86/gem5.opt \
    x86-spec-hpc-benchmarks-from-src.py --command {command}
```
"""

from gem5.components.cachehierarchies.ruby.mesi_two_level_cache_hierarchy import (
    MESITwoLevelCacheHierarchy,
)
import argparse
import time
import os

import m5
from m5.objects import Root

from gem5.utils.requires import requires
from gem5.components.boards.x86_board import X86Board
from gem5.components.memory import DualChannelDDR4_2400
from gem5.components.memory.single_channel import SingleChannelDDR3_1600
from gem5.components.processors.simple_switchable_processor import (
    SimpleSwitchableProcessor,
)
from gem5.components.processors.cpu_types import CPUTypes
from gem5.isas import ISA
from gem5.coherence_protocol import CoherenceProtocol
from gem5.resources.resource import Resource, CustomDiskImageResource

from m5.stats.gem5stats import get_simstat
from m5.util import warn

requires(
    isa_required=ISA.X86,
    coherence_protocol_required=CoherenceProtocol.MESI_TWO_LEVEL,
    kvm_required=True,
)

parser = argparse.ArgumentParser(
    description="An example configuration script to run the npb benchmarks."
)

# The only positional argument accepted is the benchmark name in this script.

benchmark_choices = ["cloverleaf", "tealeaf"]

parser.add_argument(
    "--benchmark",
    type=str,
    required=True,
    help="Input the benchmark program to execute.",
    choices=benchmark_choices,
)

core_choices = [1, 2, 4, 8]

parser.add_argument(
    "--cores",
    type=int,
    required=True,
    help="Input the number of processes to execute with",
    choices=core_choices,
)

parser.add_argument(
    "--ticks",
    type=int,
    help="Optionally put the maximum number of ticks to execute during the "
    "ROI. It accepts an integer value.",
)

args = parser.parse_args()

# Checking for the maximum number of instructions, if provided by the user.

# Setting up all the fixed system parameters here
# Caches: MESI Two Level Cache Hierarchy

cache_hierarchy = MESITwoLevelCacheHierarchy(
    l1d_size="32kB",
    l1d_assoc=8,
    l1i_size="32kB",
    l1i_assoc=8,
    l2_size="256kB",
    l2_assoc=16,
    num_l2_banks=2,
)
# Memory: Dual Channel DDR4 2400 DRAM device.
# The X86 board only supports 3 GB of main memory.

memory = DualChannelDDR4_2400(size="3GB")

# Here we setup the processor. This is a special switchable processor in which
# a starting core type and a switch core type must be specified. Once a
# configuration is instantiated a user may call `processor.switch()` to switch
# from the starting core types to the switch core types. In this simulation
# we start with KVM cores to simulate the OS boot, then switch to the Timing
# cores for the command we wish to run after boot.

processor = SimpleSwitchableProcessor(
    starting_core_type=CPUTypes.KVM,
    switch_core_type=CPUTypes.TIMING,
    num_cores=8,
)

# Here we setup the board. The X86Board allows for Full-System X86 simulations

board = X86Board(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy,
)

# Here we set the FS workload, i.e., npb benchmark program
# After simulation has ended you may inspect
# `m5out/system.pc.com_1.device` to the stdout, if any.

# After the system boots, we execute the benchmark program and wait till the
# ROI `workbegin` annotation is reached (m5_work_begin()). We start collecting
# the number of committed instructions till ROI ends (marked by `workend`).
# We then finish executing the rest of the benchmark.

# Also, we sleep the system for some time so that the output is printed
# properly.

command = (
    "echo 'Starting Test Run';"
    + "ls -lah /home/gem5/TeaLeaf_OpenMP/2d;"
    
    + "export OMP_NUM_THREADS=1;"
    + "cd /home/gem5/TeaLeaf_OpenMP/2d;"
    + "make clean;"
    + "make CC=gcc CPP=g++ KERNELS=omp4;"
    + "echo 'finished building binaries';"
    + "sudo ./tealeaf;"
    + "echo 'finished test run';"

    + "sleep 5; m5 exit;"  # switch from kvm to timing processor
    + "export OMP_NUM_THREADS={};".format(args.cores)
    + "sudo ./tealeaf;"

    + "sleep 5; m5 exit;"  # dump stats
)

board.set_kernel_disk_workload(
    # The x86 linux kernel will be automatically downloaded to the
    # `disk-image/prebuilt` directory if not already present.
    kernel=Resource(
        resource_name="x86-linux-kernel-4.19.83",
        resource_directory="disk-image/prebuilt",
    ),
    # The location of the x86 SPEC hpc 2021 image
    disk_image=CustomDiskImageResource(
        # local_path="disk-image/from-src/from-src-image/image",
        local_path="disk-image/spec-hpc/spec-hpc-image/spec-hpc",
        disk_root_partition="1"),
    readfile_contents=command,
)

# We need this for long running processes.
m5.disableAllListeners()

root = Root(full_system=True, system=board)

# sim_quantum must be set when KVM cores are used.

root.sim_quantum = int(1e9)

m5.instantiate()

# We maintain the wall clock time.

globalStart = time.time()

print("Running the simulation")
print("Using KVM cpu")

# We start the simulation.

exit_event = m5.simulate()

# The first exit_event ends with a `workbegin` cause. This means that the
# system started successfully and the execution on the program started.

if exit_event.getCause() == "m5_exit instruction encountered":

    print("Done booting Linux")
    print("Resetting stats at the start of ROI!")

    m5.stats.reset()
    start_tick = m5.curTick()

    # We have completed up to this step using KVM cpu. Now we switch to timing
    # cpu for detailed simulation.

    processor.switch()
else:
    # `workbegin` call was never encountered.

    print("Unexpected termination of simulation before ROI was reached!")
    print(
        "Exiting @ tick {} because {}.".format(
            m5.curTick(), exit_event.getCause()
        )
    )
    exit(-1)

# The next exit_event is to simulate the ROI. It should be exited with a cause
# marked by `workend`.

# Next, we need to check if the user passed a value for --ticks. If yes,
# then we limit out execution to this number of ticks during the ROI.
# Otherwise, we simulate until the ROI ends.
if args.ticks:
    exit_event = m5.simulate(args.ticks)
else:
    exit_event = m5.simulate()


# Reached the end of ROI.
# We dump the stats here.

# We exepect that ROI ends with `workend` or `simulate() limit reached`.
# Otherwise the simulation ended unexpectedly.
if exit_event.getCause() == "m5_exit instruction encountered":
    print("Dump stats at the end of the ROI!")

    m5.stats.dump()
    end_tick = m5.curTick()
elif (
    exit_event.getCause() == "simulate() limit reached"
    and args.ticks is not None
):
    print("Dump stats at the end of {} ticks in the ROI".format(args.ticks))

    m5.stats.dump()
    end_tick = m5.curTick()
else:
    print("Unexpected termination of simulation while ROI was being executed!")
    print(
        "Exiting @ tick {} because {}.".format(
            m5.curTick(), exit_event.getCause()
        )
    )
    exit(-1)

# Simulation is over at this point. We acknowledge that all the simulation
# events were successful.
print("All simulation events were successful.")
# We print the final simulation statistics.

print("Done with the simulation")
print()
print("Performance statistics:")

print("Simulated time in ROI: %.2fs" % ((end_tick - start_tick) / 1e12))
# print("Instructions executed in ROI: %d" % ((roi_insts)))
print("Ran a total of", m5.curTick() / 1e12, "simulated seconds")
print(
    "Total wallclock time: %.2fs, %.2f min"
    % (time.time() - globalStart, (time.time() - globalStart) / 60)
)

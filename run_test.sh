#!/bin/bash

trap "exit" INT

# run system configuration

adddate() {
    while IFS= read -r line; do
        printf '[%s] %s\n' "$(date)" "$line";
    done
}

b=13.soma
s=t

for cpu in 16 8 4 2 1
# for cpu in 1
do
    echo "Starting run with benchmark: $b, cpu: $cpu, size: $s"
    ./gem5/build/X86/gem5.opt x86-spec-hpc-benchmarks.py --benchmark $b --size $s --cores 16 | adddate &>> transcript.log
    cp -r m5out temp/5$b-$s-p$cpu
done
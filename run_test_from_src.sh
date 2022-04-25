#!/bin/bash

trap "exit" INT

# run system configuration

adddate() {
    while IFS= read -r line; do
        printf '[%s] %s\n' "$(date)" "$line";
    done
}

b=cloverleaf

# for cpu in 1
for cpu in 1 2 4 8
do
    echo "Starting run with benchmark: $b, cpu: $cpu"
    ./gem5.2/build/X86/gem5.opt --outdir=m5out-from-src x86-spec-hpc-benchmarks-from-src.py --benchmark $b --cores $cpu | adddate &>> transcript-from-src.log
done
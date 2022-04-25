#!/bin/bash

trap "exit" INT

b=miniweather

# run system configuration
addtag() {
    while IFS= read -r line; do
        printf '[%s]<%s_p%s> %s\n' "$(date)" "$b" "$cpu" "$line";
    done
}

mkdir archive

for cpu in 1 2 4 8
do
    echo "Starting run with benchmark: $b, cpu: $cpu" | addtag
    ./gem5/build/X86/gem5.opt --outdir=archive/$b-p$cpu x86-spec-hpc-benchmarks.py --benchmark $b --cores $cpu | addtag &>> transcript.log
done
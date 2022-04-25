#!/usr/bin/python3
import argparse
import scripts.cloverleaf as cloverleaf

parser = argparse.ArgumentParser(
    description="Build and run SPEC hpc 2021 benchmarks from their open source projects"
)

benchmark_choices = [cloverleaf.NAME]

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
    '--build',
    action="store_true",
    help="Build project without run"
)

parser.add_argument(
    '--clean',
    action="store_true",
    help="Clean project temp files"
)

args = parser.parse_args()


builder = {
    cloverleaf.NAME: cloverleaf.build
}

runner = {
    cloverleaf.NAME: cloverleaf.run
}

cleaner = {
    cloverleaf.NAME: cloverleaf.clean
}

if args.clean:
    print("Cleaning {}".format(args.benchmark))
    cleaner[args.benchmark]()
    exit()

if args.build:
    print("Building {}".format(args.benchmark))
    builder[args.benchmark]()
else:
    print("Running {} with cores={}".format(args.benchmark, args.cores))
    runner[args.benchmark](args)

#!/usr/bin/python

import os

NAME = "cloverleaf"


def build():
    os.system("cd CloverLeaf_MPI; make COMPILER=GNU")


def run(args):
    os.system("cd CloverLeaf_MPI; mpirun -np {} ./clover_leaf".format(args.cores))


def clean():
    os.system("cd CloverLeaf_MPI; make clean")

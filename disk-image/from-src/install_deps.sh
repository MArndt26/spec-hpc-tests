#!/bin/bash

# download mpich
wget https://www.mpich.org/static/downloads/3.4.1/mpich-3.4.1.tar.gz
gunzip mpich-3.4.1.tar.gz
tar xf mpich-3.4.1.tar
cd mpich-3.4.1
./configure --prefix=/home/mpich-install 2>&1 --with-device=ch3 | tee c.txt
make 2>&1 | tee m.txt
make install 2>&1 | tee mi.txt
PATH=/home/mpich-install/bin:$PATH ; export PATH
which mpicc
which mpiexec

#TODO: Add script for installing PnetCDF
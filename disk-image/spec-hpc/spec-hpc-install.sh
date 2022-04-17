# install build-essential (gcc and g++ included) and gfortran

#Compile SPEC hpc benchmark

echo "12345" | sudo apt-get install build-essential gfortran

cd /home/gem5/2d/

# FIXME: this causes illegal instructions for some reason, make clean and make again before each run
make
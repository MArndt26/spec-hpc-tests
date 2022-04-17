# install build-essential (gcc and g++ included) and gfortran

#Compile NPB

echo "12345" | sudo apt-get install build-essential gfortran

ls
pwd
cd /home/gem5/spec-hpc/
ls
pwd

# mkdir bin
make
mkdir /home/gem5/spec-hpc/bin
cp tealeaf.x /home/gem5/spec-hpc/bin/tealeaf.x
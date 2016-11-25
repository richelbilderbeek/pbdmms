#!/bin/bash
module load Qt
module load GCC/5.1.0

mkdir 1
cp lyke_peregrine 1
cd 1
sbatch ./lyke_peregrine
cd ..

mkdir 2
cp lyke_peregrine 2
cd 2
sbatch ./lyke_peregrine
cd ..

mkdir 3
cp lyke_peregrine 3
cd 3
sbatch ./lyke_peregrine
cd ..


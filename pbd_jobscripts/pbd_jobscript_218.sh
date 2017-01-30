#!/bin/bash
# Example job script

#SBATCH --time=1:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --mem=100G
#SBATCH --job-name=pbd_jobscript
#SBATCH --output=pbd_jobscript.log
module load Qt
module load GCC/5.1.0
module load R/3.3.1-foss-2016a

# Create a parameter file
../pbd_peregrine --create

for i in {1..10}
do
  sed -i "s/seed: [0-9]*/seed: $i/"
  #../pbd_peregrine pbd.txt

done



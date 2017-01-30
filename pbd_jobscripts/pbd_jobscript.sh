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
../pbd_peregrine --create
../pbd_peregrine pbd.txt

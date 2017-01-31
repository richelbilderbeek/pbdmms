#!/bin/bash
# Example job script

#SBATCH --time=12:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --mem=100G
#SBATCH --job-name=jobo_jobscript_219_run
#SBATCH --output=jobo_jobscript_219_run.log
module load Qt
module load GCC/5.1.0
module load R/3.3.1-foss-2016a
../../jobo_peregrine jobo_parameters_10.txt
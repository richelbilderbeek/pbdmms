#!/bin/bash
#SBATCH --time=12:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --mem=10G
#SBATCH --job-name=200clades
#SBATCH --output=200clades-%j.log
module load Qt
module load GCC/5.1.0
./elly_peregrine parameters_200clades.txt
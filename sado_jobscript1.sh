#!/bin/bash
#SBATCH --time=8-12:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --mem=100G
#SBATCH --job-name=kewe_sado_I
#SBATCH --mail-type=BEGIN,END
#SBATCH --mail-user=k.w.wesselink@student.rug.nl
#SBATCH --output=kewe_sado_I.log
module load Qt
module load GCC/5.1.0
./sado_peregrine_2 ../kewe_sado_test_parameters.txt
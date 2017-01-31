#!/bin/bash
#SBATCH --time=12:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --mem=1G
#SBATCH --job-name=Jorik_first_test
#SBATCH --mail-type=BEGIN,END
#SBATCH --mail-user=j.de.boer.40@student.rug.nl
#SBATCH --output=jorik_first_test.log
module load Qt
module load GCC/5.1.0
./jobo_peregrine 


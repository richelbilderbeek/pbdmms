#!/bin/bash
#SBATCH --time=8-12:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --mem=100G
#SBATCH --job-name=jobo_jobscript_episode_XV
#SBATCH --mail-type=BEGIN,END
#SBATCH --mail-user=j.de.boer.40@student.rug.nl
#SBATCH --output=jobo_episode_XV.log
module load Qt
module load GCC/5.1.0
./jobo_peregrine ../jobo_parameters_13.txt
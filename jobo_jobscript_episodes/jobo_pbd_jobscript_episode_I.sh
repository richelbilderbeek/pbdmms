#!/bin/bash
#SBATCH --time=8-12:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --mem=100G
#SBATCH --job-name=jobo_pbd_jobscript_episode_I
#SBATCH --mail-type=BEGIN,END
#SBATCH --mail-user=j.de.boer.40@student.rug.nl
#SBATCH --output=jobo_pbd_episode_I.log
module load Qt
module load GCC/5.1.0
./pbd_peregrine ../jobo_parameters/jobo_pbd_parameters.txt
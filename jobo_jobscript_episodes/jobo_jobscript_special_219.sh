#!/bin/bash
# Example job script

#SBATCH --time=12:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --mem=100G
#SBATCH --job-name=jobo_jobscript_special_219
#SBATCH --output=jobo_jobscript_special_219.log
module load Qt
module load GCC/5.1.0
module load R/3.3.1-foss-2016a

for i in {1..10}
do
  # Create a folder with the name of i
  mkdir $i

  # Replace the seed of the parameter file
  sed -i "s/seed: [0-9]*/seed: $i/" jobo_parameters_10.txt

  # Copy the parameter file into that folder
  cp jobo_parameters_10.txt $i

  # Copy the script to run into that folder
  cp jobo_jobscript_special_219_run.sh $i/to_run.sh

  # Go into that folder
  cd $i

  # Submit the job
  sbatch to_run.sh

  # Leave that folder
  cd ..
  
done

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
  # Create a folder with the name of i
  mkdir $i

  # Replace the seed of the parameter file
  sed -i "s/seed: [0-9]*/seed: $i/" pbd.txt

  # Copy the parameter file into that folder
  cp pbd.txt $i

  # Copy the script to run into that folder
  cp pbd_jobscript_219_run.sh $i/to_run.sh

  # Go into that folder
  cd $i

  # Create a symbolic link to the executable
  ln -s ../pbd_peregrine .

  # Submit the job
  sbatch to_run.sh

  # Leave that folder
  cd ..
  
done



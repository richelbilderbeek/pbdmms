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

export DISPLAY=localhost:1.0
sleep 3 # give xvfb some time to start



for i in {1..2}
do
   mkdir "sim${i}"
   cd "sim$i"
   ln ../sado_jobscript1.sh
   ln ../sado_peregrine_2
   cp ../kewe_sado_test_parameters.txt
   echo "seed " i >> kewe_sado_test_parameters.txt
   # Do a run
   xvfb-run -a ./sado_peregrine_2 kewe_sado_test_parameters.txt 

done


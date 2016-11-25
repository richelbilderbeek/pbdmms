#!/bin/bash
#SBATCH --time=0:01:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --mem=1G
#SBATCH --job-name=send_lyke_an_email
#SBATCH --output=send_lyke_an_email.log
#SBATCH --mail-type=END
#SBATCH --mail-user= l.e.stuurman@student.rug.nl
echo "Send me an email"

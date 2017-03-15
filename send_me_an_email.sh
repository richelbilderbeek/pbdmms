#!/bin/bash
# Called from scripts folder by run.sh
#SBATCH --time=0:01:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --mem=1G
#SBATCH --job-name=send_me_an_email
#SBATCH --output=send_me_an_email.log
#SBATCH --mail-type=END
#SBATCH --mail-user=r.j.c.bilderbeek@rug.nl
echo "Send me an email"

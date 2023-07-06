#!/bin/bash
#SBATCH --job-name=ezTrim
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=dmendoza@tgen.org
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=20
#SBATCH --mem=60gb
#SBATCH --time=24:00:00
#SBATCH --output=ezTrim_%j.log

time ./main
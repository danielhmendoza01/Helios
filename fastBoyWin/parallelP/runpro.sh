#!/bin/bash
#SBATCH --job-name=job_P1
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=dmendoza@tgen.org
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=20
#SBATCH --mem=60gb
#SBATCH --time=24:00:00
#SBATCH --output=job_test_%j.log

time ./main
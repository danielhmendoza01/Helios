#!/bin/bash
#SBATCH --job-name=job_test
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=dmendoza@tgen.org
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=16gb
#SBATCH --time=04:00:00
#SBATCH --output=job_test_%j.log
echo "Running"
./main

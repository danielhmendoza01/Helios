#!/bin/bash
#SBATCH --job-name=job_testR1
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=dmendoza@tgen.org
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=20
#SBATCH --mem=60gb
#SBATCH --time=24:00:00
#SBATCH --output=job_test_%j.log
echo "Running Script"
echo "Defining Variables"
# File Paths
fastqFile="/path/to/file1.txt"
outputFile="/path/to/file2.txt"
logFile="/path/to/file3.txt"

# Variable Arguments
baseSize=50
pattern="CGTGTGC"
./main

#!/bin/bash
#SBATCH --job-name=ezTrim
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=dmendoza@tgen.org
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=10
#SBATCH --mem=8gb
#SBATCH --time=02:00:00
#SBATCH --output=ezTrim_%j.log
readFile="/scratch/dmendoza/logs/testFiles/8276.fastq.gz"
pattern="AGATCGGAAGAGCACACGTCTGAACTCCAGTCA"
tempPath="/scratch/dmendoza/logs/files/fastqFiles/temp/"
outputPath="/scratch/dmendoza/logs/files/fastqFiles/"
outFileName=""
windowSize=5
time ./ezTrim -i $readFile -p $pattern -t $tempPath -h $outputPath -w $windowSize -v 

#!/bin/bash
#SBATCH --job-name=ezTrim_qual-seqtk_ezTrim
#SBATCH --mail-type=ALL
#SBATCH --mail-user=dmendoza@tgen.org
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=20
#SBATCH --mem=60gb
#SBATCH --time 24:00:00
#SBATCH --output=/scratch/dmendoza/logs/files/logs/ezTrim-qual.log

module load python

for file in eztrim_metrics.py

do

	python ${file} & 

done
wait


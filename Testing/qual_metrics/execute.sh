#!/bin/bash
#SBATCH --job-name=ezTrim_qual-notPar
#SBATCH --mail-type=ALL
#SBATCH --mail-user=dmendoza@tgen.org
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=20
#SBATCH --mem=60gb
#SBATCH --time 24:00:00
#SBATCH --output=/scratch/dmendoza/logs/files/logs/ezTrim_qual-notPar_%j.log

module load python

files = ("seqtk_metrics.py" "seqtk_metrics.py" "seqtk_metrics.py" "seqtk_metrics.py")

for file in files[@]

do

	python ${file} 

done
wait


#!/bin/bash

# Specify your directory here
dir="$1"
dir2="$2"
outFile="$3"

# Concatenate the files in alphabetical order and gzip them
echo $dir2$outFile
#find $dir -name "post*.fastq.gz" -print0 | sort -z | xargs -0 cat > $dir2$outFile
find $dir -name "post*.fastq" -print0 | sort -z | xargs -0 cat | pigz > $dir2$outFile
find $dir -name "post*.fastq" -exec rm {} +



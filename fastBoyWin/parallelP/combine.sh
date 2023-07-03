#!/bin/bash

# Specify your directory here
dir="$1"
dir2="$2"

# Concatenate the files in alphabetical order and gzip them
find $dir -name "*.fastq" -print0 | sort -z | xargs -0 cat | pigz > $dir2/combined.fastq.gz


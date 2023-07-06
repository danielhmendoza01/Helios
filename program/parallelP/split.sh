#!/bin/bash
readFile="$1"
inPath="$2"
pigz -d -c "$readFile" | split -l 20000000 -d - "$inPath/split" --additional-suffix=.fastq
#!/bin/bash
readFile="$1"
inPath="$2"
echo "$readFile"
pigz -d -c "$readFile" | split -l 19000000 -d - "$inPath/split" --additional-suffix=.fastq
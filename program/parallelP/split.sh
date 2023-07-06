#!/bin/bash

readFile="$1"
inPath="$2"

echo "$readFile"

gunzip < "$readFile" | \
awk -v sizeLimit=$((4*1024*1024*1024)) -v outDir="$inPath" '
BEGIN {
    suffix=1;
    outputFile=outDir "/split" sprintf("%02d", suffix) ".fastq";
    sizeCounter=0;
    lineCounter=0;
}
{
    line=$0 "\n";
    lineSize=length(line);
    sizeCounter+=lineSize;
    lineCounter++;
    printf "%s", line > outputFile;
    if (sizeCounter >= sizeLimit && lineCounter % 4 == 0) {
        close(outputFile);
        suffix++;
        outputFile=outDir "/split" sprintf("%02d", suffix) ".fastq";
        sizeCounter=0;
        lineCounter=0;
    }
}'

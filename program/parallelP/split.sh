#!/bin/bash
readFile="$1"
inPath="$2"
echo "$readFile"

# Get the file name
file="$1"

# Create a sample of the first 1000 lines of the gzipped file
pigz -d -c "$file" | head -1000 > 1000-line-sample.txt

# Get the size of the uncompressed sample file in bytes
size_sample_uncompressed=$(ls -ls 1000-line-sample.txt | awk '{ print $6 }')

# Compress the sample file
pigz 1000-line-sample.txt

# Get the size of the sample file and the original gzipped file in bytes
size_sample_compressed=$(ls -ls 1000-line-sample.txt.gz | awk '{ print $6 }')
size_file=$(ls -ls "$file" | awk '{ print $6 }')

# Calculate the compression ratio
compression_ratio=$(echo "$size_sample_uncompressed/$size_sample_compressed" | bc -l)

# Calculate the approximate line count
line_count=$(echo "$size_file/$size_sample_compressed*1000*$compression_ratio" | bc)

# Print the approximate line count
echo "Approximate line count: $line_count"
num_threads=$(nproc)
result=$(echo "$line_count / $num_threads" | bc)
# Could optimize for available memory
while [ $result -gt 20000000 ];
do
    result=$(echo "$result / $num_threads" | bc)
done
result=$(echo "$result / 100 * 100" | bc)
echo "$result lines"

# Clean up the sample files
rm 1000-line-sample.txt.gz

#split
pigz -d -c "$readFile" | split -l $result -d -a 3 - "$inPath/split" --additional-suffix=.fastq
ezTrim
Quality Trimming Tool

Features
Quality Trim
Trim adapters
Compressed output

Set up
Compile with:
g++ -std=c++2a main.cpp -o main -fopenmp -lz

Using ezTrim
-i input_file -p pattern [-t temp_path] [-o out_file_name] [-h output_path] [-w window_size] [-b base_size] [-t trim_threshold] [-v 3_to_5_prime_trim]

Required                default                 Explanation
-i input_file           None                    File that needs to be trimmed (file.fastq.gz)
-p pattern              None                    The adapter pattern that needs to be removed
Optional
-t temp_path            current directory       A directory path can be given to store temporary files
-o out_file_name        current directory       A file name can be given to the output file
-h output_path          current directory       A directory path can be given to store temporary files
-w window_size          4                       An alternate size can be given
-b base_size            50                      An alternate base size can be given
-t trim_threshold       20                      an alternate threshold can be given
-v 3_to_5_prime_trim    forward                 Can do 3 to 5 prime end trimmming



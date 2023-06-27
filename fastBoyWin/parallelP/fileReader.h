#pragma once
#include <iostream>
#include <fstream>
#include <zlib.h>
#include <cstring>
#include <vector>
using namespace std;

//Function to create a GZ File that can be written
gzFile createGzFile(const std::string& filename) {
    gzFile file = gzopen(filename.c_str(), "wb");
    if (file == nullptr) {
        std::cerr << "Error opening " << filename << " for writing." << std::endl;
        exit(1);
    }
    return file;
}
//function to write to the gzFile
//It takes in a char vector and on a new line the data in the vector is added to file
void writeDataToGzFile(gzFile file, std::vector<char> data) {
    data.push_back('\n');
    gzwrite(file, data.data(), data.size());
}
void writeBufferToGzFile(gzFile file, std::vector<char> data){
    gzwrite(file, data.data(), data.size());
}
void combineGzFiles(const std::string& output_file, const std::vector<std::string>& gz_files) {
    std::ofstream output(output_file, std::ios::binary);
    if (!output) {
        std::cerr << "Failed to open output file: " << output_file << std::endl;
        return;
    }

    for (const auto& gz_file : gz_files) {
        gzFile input = gzopen(gz_file.c_str(), "rb");
        if (!input) {
            std::cerr << "Failed to open input file: " << gz_file << std::endl;
            return;
        }

        char buffer[1024];
        int num_read = 0;
        while ((num_read = gzread(input, buffer, sizeof(buffer))) > 0) {
            output.write(buffer, num_read);
        }

        gzclose(input);
    }

    output.close();
}
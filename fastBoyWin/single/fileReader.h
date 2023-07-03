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
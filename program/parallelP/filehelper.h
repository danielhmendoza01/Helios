#pragma once
#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <zlib.h>
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

void gzWriteStringToGzFile(gzFile file, string data){
    gzwrite(file, data.c_str(), static_cast<int>(data.length()));
}

void gzWriteIntToGzFile(gzFile file, int data){
    gzwrite(file, to_string(data).c_str(),static_cast<int>(to_string(data).length()));
}

void checkDuplicates(unordered_set<std::string>& identifierLines, string line){
    if (identifierLines.count(line) > 0) {
            cout << "Duplicate identifier line: " + line << endl;
            exit(0);
            // Handle duplicate identifier line as needed
    } else {
        // Add the identifier line to the unordered_set
        identifierLines.insert(line);
    }
}
void validate_vector(const std::vector<char>& vec) {
    for(const auto& ch : vec) {
        if(ch != 'N' && ch != 'A' && ch != 'T' && ch != 'G' && ch != 'C') {
            cout << "*****Invalid Base*****"<<endl;
            exit(0);
        }
    }
}
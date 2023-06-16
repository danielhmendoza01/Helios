#pragma once
#include <iostream>
#include <fstream>
#include <zlib.h>
#include <cstring>
#include <vector>
using namespace std;

void gzWriteStringToGzFile(gzFile file, string data){
    gzwrite(file, data.c_str(), static_cast<int>(data.length()));
}

void gzWriteIntToGzFile(gzFile file, int data){
    gzwrite(file, to_string(data).c_str(),static_cast<int>(to_string(data).length()));
}

void gzWriteCut(gzFile file, int data){
    gzWriteStringToGzFile(file, "Cut: ");
    gzWriteIntToGzFile(file, data);
    gzWriteStringToGzFile(file, "\n");
}

void checkDuplicates(unordered_set<std::string> identifierLines, string line, gzFile file){
    if (identifierLines.count(line) > 0) {
            gzWriteStringToGzFile(file, "Duplicate identifier line: " + line);
            // Handle duplicate identifier line as needed
    } else {
        // Add the identifier line to the unordered_set
        identifierLines.insert(line);
    }
}
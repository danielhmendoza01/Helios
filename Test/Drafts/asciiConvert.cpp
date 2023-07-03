// ISSUES WITH BUFFER


#include <iostream>
#include <fstream>
#include <zlib.h>
#include <cstring>
#include <vector>
using namespace std;

gzFile createGzFile(const string& filename) {
    gzFile file = gzopen(filename.c_str(), "wb");
    if (file == nullptr) {
        cerr << "Error opening " << filename << " for writing." << endl;
        exit(1);
    }
    return file;
}
void writeDataToGzFile(gzFile file, vector<char> data) {
    data.push_back('\n');
    gzwrite(file, data.data(), data.size());
}
int windowSlide(vector<int> numLine){
    if (numLine.empty()) {
        return -1; // Handle empty vector case
    }
    
    int i = 0;
    int avg = 0;
    int windowSize = 5;
    while (i + windowSize <= numLine.size()){
        int l = i + windowSize;
        for (int index = i; index < l; index++){
            avg += numLine[index];
        }
        avg = avg / windowSize;
        if (avg < 20){
            return i;
        }
        i++;
        avg = 0;
    }
    return -1;
}


int main() {
    const char* filename = "/scratch/dmendoza/Sample_COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY/test.fastq.gz";
    gzFile outputFile = createGzFile("output.fastq.gz");
    gzFile file = gzopen(filename, "r");
    if (file == nullptr) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    char buffer[2000];
    int bytesRead;
    int lineCount = 0;
    vector<char> id;
    vector<char> sequence;
    vector<char> plusLine;
    plusLine.push_back('+');
    vector<char> asciiLine;
    vector<int> numericLine;

    while ((bytesRead = gzread(file, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytesRead; i++) {
            if (buffer[i] == '\n') {
                lineCount++;
                if (lineCount % 4 == 0) {
                    // Process the ASCII line
                    for (char c : asciiLine) {
                        int numericalValue = static_cast<int>(c) - 33;
                        numericLine.push_back(numericalValue);
                    }
                    int cutOff = windowSlide(numericLine);
                    if (cutOff != -1){
                        asciiLine.erase(asciiLine.begin()+ cutOff, asciiLine.end());
                        sequence.erase(sequence.begin() + cutOff, sequence.end());
                    }
                    writeDataToGzFile(outputFile, id);
                    writeDataToGzFile(outputFile, sequence);
                    writeDataToGzFile(outputFile, plusLine);
                    writeDataToGzFile(outputFile, asciiLine);

                    // Clear the vectors for the next set of lines
                    id.clear();
                    sequence.clear();
                    asciiLine.clear();
                    numericLine.clear();
                }
                else if (lineCount % 4 == 1) {
                    // Store the ID line
                    for (char c : buffer) {
                        id.push_back(c);
                    }
                }
                else if (lineCount % 4 == 3) {
                    // Store the ASCII characters
                    for (char c : buffer) {
                        asciiLine.push_back(c);
                    }
                }
            }
            else {
                if (lineCount % 4 == 1) {
                    // Store the ID characters
                    id.push_back(buffer[i]);
                }
                else if (lineCount % 4 == 2) {
                    // Store the sequence characters
                    sequence.push_back(buffer[i]);
                }
                else if (lineCount % 4 == 3) {
                    // Store the ASCII characters
                    asciiLine.push_back(buffer[i]);
                }
            }
        }
    }

    gzclose(file);
    gzclose(outputFile);
    return 0;
}

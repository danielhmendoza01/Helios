#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <zlib.h>
#include <algorithm>
#include <iterator>
#include <omp.h>
using namespace std;

// Function to create a GZ File that can be written
gzFile createGzFile(const std::string& fileName) {
    gzFile gzFileObj = gzopen(fileName.c_str(), "wb");
    if (gzFileObj == nullptr) {
        std::cerr << "Error opening " << fileName << " for writing." << std::endl;
        exit(1);
    }
    return gzFileObj;
}

// Function to write data to the gzFile
// It takes in a char vector and on a new line the data in the vector is added to file
void writeDataToGzFile(gzFile gzFileObj, std::vector<char> data) {
    data.push_back('\n');
    gzwrite(gzFileObj, data.data(), data.size());
}

int main(){
    const unsigned long GIGABYTE = 1024 * 1024 * 1024;
    const char* sourceFileName = "/scratch/dmendoza/Sample_COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY/COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY_TTGGACGT_L004_R1_001.fastq.gz";
    gzFile sourceFile = gzopen(sourceFileName, "r");
    if (sourceFile == nullptr) {
        cerr << "Error opening file: " << sourceFileName << endl;
        return 1;
    }
    vector<gzFile> tempFiles;
    //gzFile tempFiles[11]; // Array of temporary gzFiles
    int numThreads = omp_get_max_threads();
    for(int i=0; i < numThreads; i++){
        gzFile tempFile = createGzFile("/scratch/dmendoza/Helios_Project/fastBoyWin/files/fastqFiles/tTest/temp" + to_string(i) + ".fastq.gz");
        tempFiles.push_back(tempFile);
    }

    int bytesRead;
    size_t bufferSize = 1048576; // Size of buffer, 1MB
    std::vector<char> buffer(bufferSize); // Buffer to read data into
    int tempFileID = 0; // Counter for temporary files
    unsigned long totalBytes = 0;
    // Buffers for temporary and write data
    vector<char> tempDataBuffer;
    vector<char> writeDataBuffer;

    int readCount = 1;
    while ((bytesRead = gzread(sourceFile, buffer.data(), bufferSize - tempDataBuffer.size())) > 0) {
        // Check end of buffer and concatenate with temporary data
        if(!tempDataBuffer.empty()){
            buffer.insert(buffer.begin(), tempDataBuffer.begin(), tempDataBuffer.end());
            buffer.resize(bytesRead + tempDataBuffer.size());
            tempDataBuffer.clear();
        }
        cout << "BEGIN"<<endl;
        totalBytes += bytesRead;
        cout << totalBytes << endl;
        auto endBufferIter = buffer.rbegin();
        auto plusSignIter = find(endBufferIter, buffer.rend(), '+'); 
        auto bufferIndex = distance(plusSignIter, buffer.rend()) - 1;
        
        // Search for valid '+' sign
        while(true){
            plusSignIter = find(endBufferIter, buffer.rend(), '+');
            bufferIndex = distance(plusSignIter, buffer.rend()) - 1;
            if(buffer[bufferIndex+1]=='\n' && buffer[bufferIndex-1] == '\n'){
                break;
            }
            auto distanceToEnd = buffer.size() - bufferIndex;
            endBufferIter += distanceToEnd;
        }

        // n is index of '+', search for next '\n'
        auto nextNewLineIter = find(buffer.begin() + bufferIndex + 2, buffer.end(), '\n');
        auto nextNewLineIndex = distance(buffer.begin(), nextNewLineIter);
        
        if(nextNewLineIter != buffer.end()){
            cout<<"CASE1"<<endl;
            tempDataBuffer = {buffer.begin() + nextNewLineIndex+1, buffer.end()};
            writeDataBuffer = {buffer.begin(), buffer.begin()+nextNewLineIndex};
        }
        else{
            cout<<"CASE2"<<endl;
            int count = 0;
            auto newlineIter = buffer.rbegin();
            while (newlineIter != buffer.rend()) {
                if (*newlineIter == '\n') {
                    count++;
                    if (count == 4) {
                        break;
                    }
                }
                newlineIter++;
            }
            if (count == 4) {
                auto newlineIndex = distance(newlineIter, buffer.rend()) - 1;
                // The index of the fourth newline character is stored in 'newlineIndex'
                tempDataBuffer = {buffer.begin() + newlineIndex+1, buffer.end()};
                writeDataBuffer = {buffer.begin(), buffer.begin()+newlineIndex};

                for(int j = 0; j < tempDataBuffer.size(); j++){
                    cout << tempDataBuffer[j];
                }
            } else {
                // The fourth newline character was not found
                cout << "********ERROR*********";
                exit(0);
            }

        }
        writeDataToGzFile(tempFiles[tempFileID],writeDataBuffer);
        writeDataBuffer.clear();
        cout << "END" << endl;
        if(totalBytes > GIGABYTE){
            tempFileID++;
            exit(0);
        }
        if(tempFileID > tempFiles.size()-1){
            gzFile tempFile = createGzFile("/scratch/dmendoza/Helios_Project/fastBoyWin/files/fastqFiles/tTest/temp" + to_string(tempFileID) + ".fastq.gz");
            tempFiles.push_back(tempFile);
        }
    }

    gzclose(sourceFile);
    for(int i = 0; i <10; i++){
        gzclose(tempFiles[i]);
    }

    return 0;
}

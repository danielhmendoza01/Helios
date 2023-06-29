#include "fileReader.h" //possible error location
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


void fileSplit(string inFile, vector<string>& tempFilesPath, vector<string>& tempFilesOut){
    const uint64_t GIGABYTE = 1024ULL * 1024 * 1024;
    const char* fileName = inFile.data();
    gzFile sourceFile = gzopen(fileName, "r");
    if (sourceFile == nullptr) {
        cerr << "Error opening file: " << fileName << endl;
        exit(1);
    }
    vector<gzFile> tempFiles;
    //gzFile tempFiles[11]; // Array of temporary gzFiles
    int numThreads = omp_get_max_threads();
    for(int i=0; i < numThreads; i++){
        gzFile tempFile = createGzFile("/scratch/dmendoza/logs/files/fastqFiles/tTest/temp" + to_string(i) + ".fastq.gz");
        //gzFile tempOutFile =createGzFile("/scratch/dmendoza/logs/files/fastqFiles/tTestOut/temp" + to_string(i) + ".fastq.gz");

        tempFiles.push_back(tempFile);
        tempFilesPath.push_back("/scratch/dmendoza/logs/files/fastqFiles/tTest/temp" + to_string(i) + ".fastq.gz");
        tempFilesOut.push_back("/scratch/dmendoza/logs/files/fastqFiles/tTestOut/temp" + to_string(i) + ".fastq.gz");
        
    }

    int bytesRead;
    uint64_t bufferSize = (1024); // Size of buffer, 1MB is 1048576
    std::vector<char> buffer(bufferSize); // Buffer to read data into
    int tempFileID = 0; // Counter for temporary files
    uint64_t totalBytes = 0;
    // Buffers for temporary and write data
    vector<char> tempDataBuffer;
    //vector<char> writeDataBuffer;

    int readCount = 1;
    cout << "Starting loop" << endl;
    while ((bytesRead = gzread(sourceFile, buffer.data(), bufferSize - tempDataBuffer.size())) > 0) {
        totalBytes += bytesRead;
        // Check end of buffer and concatenate with temporary data
        if(!tempDataBuffer.empty()){
            buffer.insert(buffer.begin(), tempDataBuffer.begin(), tempDataBuffer.end());
            //buffer.resize(bytesRead + tempDataBuffer.size());
            tempDataBuffer.clear();
        }
        if(totalBytes > GIGABYTE*3){
            /*******/
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
                tempDataBuffer = {buffer.begin() + nextNewLineIndex+1, buffer.end()};
                writeBufferToGzFile(tempFiles[tempFileID],{buffer.begin(), buffer.begin()+nextNewLineIndex});
            }
            else{
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
                    writeBufferToGzFile(tempFiles[tempFileID],{buffer.begin(), buffer.begin()+newlineIndex});
                } else {
                    // The fourth newline character was not found
                    cout << "********ERROR*********";
                    exit(0);
                }

            }
            /*******/
            gzclose(tempFiles[tempFileID]);
            tempFileID++; 
            totalBytes=0;
            if(tempFileID > tempFiles.size()-1){
                gzFile tempFile = createGzFile("/scratch/dmendoza/logs/files/fastqFiles/tTest/temp" + to_string(tempFileID) + ".fastq.gz");
                tempFiles.push_back(tempFile);
            }            
        }
        else{
            gzwrite(tempFiles[tempFileID], buffer.data(), buffer.size());

        }
        //writeBufferToGzFile(tempFiles[tempFileID],writeDataBuffer);
        //writeDataBuffer.clear();
    }
    gzclose(sourceFile);
}
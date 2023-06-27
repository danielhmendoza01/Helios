#include "fileReader.h"
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
    int numThreads = omp_get_max_threads();
    for(int i=0; i < numThreads; i++){
        gzFile tempFile = createGzFile("/scratch/dmendoza/logs/files/fastqFiles/tTest/temp" + to_string(i) + ".fastq.gz");
        tempFiles.push_back(tempFile);
        tempFilesPath.push_back("/scratch/dmendoza/logs/files/fastqFiles/tTest/temp" + to_string(i) + ".fastq.gz");
        tempFilesOut.push_back("/scratch/dmendoza/logs/files/fastqFiles/tTestOut/temp" + to_string(i) + ".fastq.gz");
    }

    int bytesRead;
    uint64_t bufferSize = (GIGABYTE); 
    vector<char> buffer(bufferSize); 
    int tempFileID = 0;
    uint64_t totalBytes = 0;

    int readCount = 1;
    cout << "Starting loop" << endl;
    while ((bytesRead = gzread(sourceFile, &buffer[0], bufferSize)) > 0) {
        cout << "LOOP" <<endl;
        totalBytes += bytesRead;

        /*******/
        if(totalBytes > GIGABYTE*2){
            cout << "IN" <<endl;
            auto plusSignIter = find(buffer.rbegin(), buffer.rend(), '+'); 
            auto bufferIndex = distance(plusSignIter, buffer.rend()) - 1;

            while(true){
                plusSignIter = find(plusSignIter + 1, buffer.rend(), '+');
                bufferIndex = distance(plusSignIter, buffer.rend()) - 1;
                if(buffer[bufferIndex+1]=='\n' && buffer[bufferIndex-1] == '\n'){
                    break;
                }
            }

            auto nextNewLineIter = find(buffer.begin() + bufferIndex + 2, buffer.begin() + bytesRead, '\n');
            auto nextNewLineIndex = distance(buffer.begin(), nextNewLineIter);

            if (nextNewLineIter != buffer.begin() + bytesRead) {
                writeDataToGzFile(tempFiles[tempFileID], {buffer.begin(), nextNewLineIter + 1});
                copy(nextNewLineIter + 1, buffer.begin() + bytesRead, buffer.begin());
                bytesRead = buffer.begin() + bytesRead - nextNewLineIter - 1;
            }
            else {
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
                    writeDataToGzFile(tempFiles[tempFileID], {buffer.begin(), buffer.begin() + newlineIndex + 1});
                    copy(buffer.begin() + newlineIndex + 1, buffer.begin() + bytesRead, buffer.begin());
                    bytesRead = buffer.begin() + bytesRead - buffer.begin() - newlineIndex - 1;
                } else {
                    cout << "********ERROR*********";
                    exit(0);
                }
            }
            gzclose(tempFiles[tempFileID]);
            tempFileID++;
            totalBytes=0;
            if(tempFileID > tempFiles.size()-1){
                gzFile tempFile = createGzFile("/scratch/dmendoza/Helios_Project/fastBoyWin/files/fastqFiles/tTest/temp" + to_string(tempFileID) + ".fastq.gz");
                tempFiles.push_back(tempFile);
            }
        }
        else{
            writeDataToGzFile(tempFiles[tempFileID], buffer);
        }
    }
    gzclose(sourceFile);
}

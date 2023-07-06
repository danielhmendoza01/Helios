#include "mainVal.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <zlib.h>
#include <omp.h>
#include <dirent.h>
using namespace std;

string readFile = "/scratch/dmendoza/Scripts/8276-200M.fastq.gz";
string inPath = "/scratch/dmendoza/logs/files/fastqFiles/tTest/";
string outPath = "/scratch/dmendoza/logs/files/fastqFiles/tTestOut/";
string combPath = "/scratch/dmendoza/logs/files/fastqFiles/";
vector<string> fastqFiles;
vector<string> outFiles;

int main(){
    Timer splitTimer;
    Timer trimTimer;
    Timer combineTimer;
    gzFile logFile = createGzFile("/scratch/dmendoza/logs/files/logs/logT1.txt.gz");

    //variables trackes across all files
    int numTrimmed = 0;
    int adaptRemov = 0;
    long totalReads = 0;
    cout <<"*****SPLITING*****"<<endl;
    //split
    splitTimer.start();
    std::string command = "bash split.sh " + readFile + " " + inPath;
    system(command.c_str());

    splitTimer.stop();
    cout << "Split ";
    splitTimer.printElapsedTime();

    // Open the directory
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(inPath.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = ent->d_name;
            if (filename.length() > 6 && filename.substr(filename.length() - 6) == ".fastq") {
                fastqFiles.push_back(inPath + filename);
                outFiles.push_back(outPath + filename);
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Unable to open directory: " << inPath << std::endl;
    }

    cout << "Attempting parallel trim" <<endl;
    trimTimer.start();
    #pragma omp parallel for
    for(int i =0; i < fastqFiles.size(); i++){
        trim(fastqFiles[i], outFiles[i], logFile, numTrimmed, adaptRemov, totalReads);
    }
    trimTimer.stop();
    cout << "Trim ";
    trimTimer.printElapsedTime();
    cout << "Success! Combining Temp Files" <<endl;
    combineTimer.start();


    std::string command2 = "bash combine.sh " + outPath + " " + combPath;
    system(command2.c_str());

    cout << "Combining " << endl;
    combineTimer.stop();

    gzclose(logFile);
    //end program
    cout << "-----DONE-----" << endl;
    cout << "-----Summery-----" << endl;
    cout << "Adapters removed: " << adaptRemov << endl;
    cout << "Read trimmed: " << numTrimmed << endl;
    cout << "Total reads: " << totalReads;
    //Summery
    return 0;
}
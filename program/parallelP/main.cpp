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
#include <unistd.h>
#include <regex>
using namespace std;

int main(int argc, char* argv[]){
    vector<string> fastqFiles;
    vector<string> outFiles;
    Timer splitTimer;
    Timer trimTimer;
    Timer combineTimer;
    gzFile logFile = createGzFile("/scratch/dmendoza/logs/files/logs/logT1.txt.gz"); //Works ok

    string readFile;
    string pattern;
    string tempPath = "./";
    string outputPath = "./";
    string outFileName = "combined.fastq.gz";
    //Default Values
    int windowSize = 4;
    int baseSize = 50;
    int threshold = 20;
    bool reverseQTrim = false;
    string errorMessage = " -i input_file -p pattern [-t temp_path] [-o out_file_name] [-h output_path] [-w window_size] [-b base_size] [-t trim_threshold] [-v 3_to_5_prime_trim]";
    //Flags
    int opt;
    while ((opt = getopt(argc, argv, "i:t:o:p:h:w:b:r:v")) != -1) {
        switch (opt) {
            case 'i':
                readFile = optarg;
                break;
            case 't':
                tempPath = optarg;
                break;
            case 'o':
                outFileName = optarg;
                break;
            case 'h':
                outputPath = optarg;
                break;
            case 'p':
                pattern = optarg;
                break;
            case 'w':
                windowSize = stoi(optarg);
                break;
            case 'b':
                baseSize = stoi(optarg);
                break;
            case 'r':
                threshold = stoi(optarg);
                break;
            case 'v':
                reverseQTrim = true;
            default:
                cerr << "Usage: " << argv[0] << errorMessage << endl;
                return 1;
        }
    }
    //Flag error handler
    if (readFile.empty()) {
        cerr << "Input file not provided. Usage: " << argv[0] << errorMessage << endl;
        return 1;
    }
    if (pattern.empty()){
        cerr << "Pattern not provided. Usage: " << argv[0] << errorMessage << endl;
        return 1;
    }

    ifstream inputFile(readFile);
    if (!inputFile) {
        cerr << "Failed to open input file: " << readFile << endl;
        return 1;
    }

    cout << "Input file: " << readFile << endl;
    cout << "Temp path: " << tempPath << endl;
    cout << "Output path: " << outputPath << endl;

    //variables trackes across all files
    int numTrimmed = 0;
    int adaptRemov = 0;
    long totalReads = 0;
    cout <<"-----SPLITING-----"<<endl;
    splitTimer.start();
    string command = "bash split.sh " + readFile + " " + tempPath;
    system(command.c_str());

    splitTimer.stop();
    cout << "Split ";
    splitTimer.printElapsedTime();

    // Open the directory
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(tempPath.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            string filename = ent->d_name;
            if (filename.length() > 6 && filename.substr(filename.length() - 6) == ".fastq") {
                fastqFiles.push_back(tempPath + filename);
                
                outFiles.push_back(tempPath + "post" + filename[5] +filename[6] + ".fastq");
            }
        }
        closedir(dir);
    } else {
        cerr << "Unable to open directory: " << tempPath << endl;
    }
    
    cout << "Attempting parallel trim" <<endl;
    trimTimer.start();
    #pragma omp parallel for
    for(int i =0; i < fastqFiles.size(); i++){
        trim(fastqFiles[i], outFiles[i], logFile, numTrimmed, adaptRemov, totalReads, pattern, windowSize, baseSize, threshold, reverseQTrim);
    }
    trimTimer.stop();
    cout << "Trim ";
    trimTimer.printElapsedTime();
    cout << "Success! Combining Temp Files" <<endl;
    combineTimer.start();
    string command2 = "bash combine.sh " + tempPath + " " + outputPath + " " + outFileName;
    system(command2.c_str());
    combineTimer.stop();
    cout << "Combining ";
    combineTimer.printElapsedTime();
    

    gzclose(logFile);
    //end program
    cout << "-------DONE-------" << endl;
    cout << "------Summery-----" << endl;
    cout << "Adapters removed: " << adaptRemov << endl;
    cout << "Read trimmed: " << numTrimmed << endl;
    cout << "Total reads: " << totalReads << endl;
    //Summery
    return 0;
}
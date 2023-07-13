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
    //string readFile = "/scratch/dmendoza/logs/testFiles/8276-200M.fastq.gz";
    vector<string> fastqFiles;
    vector<string> outFiles;
    Timer splitTimer;
    Timer trimTimer;
    Timer combineTimer;
    gzFile logFile = createGzFile("/scratch/dmendoza/logs/files/logs/logT1.txt.gz"); //This needs to be reworked

    string readFile;
    string pattern;
    string tempPath = "./"; // "./"
    string outputPath = "./"; // "./"
    string outFileName = "combined.fastq.gz";
    int windowSize = 4;
    int baseSize = 50;
    int threshold = 20;


    int opt;
    while ((opt = getopt(argc, argv, "i:t:o:p:h:w:b:r")) != -1) {
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
            default:
                std::cerr << "Usage: " << argv[0] << " -i input_file [-t temp_path] [-h output_path]" << std::endl;
                return 1;
        }
    }

    if (readFile.empty()) {
        std::cerr << "Input file not provided. Usage: " << argv[0] << " -i input_file -p pattern [-t temp_path] [-h output_path]" << std::endl;
        return 1;
    }
    if (pattern.empty()){
        cerr << "Pattern not provided. Usage: " << argv[0] << " -i input_file -p pattern [-t temp_path] [-h output_path]" << endl;
        return 1;
    }

    std::ifstream inputFile(readFile);
    if (!inputFile) {
        std::cerr << "Failed to open input file: " << readFile << std::endl;
        return 1;
    }

    std::cout << "Input file: " << readFile << std::endl;
    std::cout << "Temp path: " << tempPath << std::endl;
    std::cout << "Output path: " << outputPath << std::endl;

    //variables trackes across all files
    int numTrimmed = 0;
    int adaptRemov = 0;
    long totalReads = 0;
    cout <<"-----SPLITING-----"<<endl;
    splitTimer.start();
    std::string command = "bash split.sh " + readFile + " " + tempPath;
    system(command.c_str());

    splitTimer.stop();
    cout << "Split ";
    splitTimer.printElapsedTime();

    // Open the directory
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(tempPath.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = ent->d_name;
            if (filename.length() > 6 && filename.substr(filename.length() - 6) == ".fastq") {
                fastqFiles.push_back(tempPath + filename);
                
                outFiles.push_back(tempPath + "post" + filename[5] +filename[6] + ".fastq");
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Unable to open directory: " << tempPath << std::endl;
    }
    
    cout << "Attempting parallel trim" <<endl;
    trimTimer.start();
    #pragma omp parallel for
    for(int i =0; i < fastqFiles.size(); i++){
        trim(fastqFiles[i], outFiles[i], logFile, numTrimmed, adaptRemov, totalReads, pattern, windowSize, baseSize, threshold);
    }
    trimTimer.stop();
    cout << "Trim ";
    trimTimer.printElapsedTime();
    cout << "Success! Combining Temp Files" <<endl;
    combineTimer.start();
    std::string command2 = "bash combine.sh " + tempPath + " " + outputPath + " " + outFileName;
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
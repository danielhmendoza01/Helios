#include "mainVal.h"
#include "fileSplit.h"
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

string inFile = "/scratch/dmendoza/Sample_COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY/COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY_TTGGACGT_L004_R1_001.fastq.gz";
string inPath = "/scratch/dmendoza/Sample_COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY/filesSplit/";
string outPath = "/scratch/dmendoza/logs/files/fastqFiles/tTestOut";
vector<string> fastqFiles;
vector<string> outFiles;

int main(){
    gzFile logFile = createGzFile("/scratch/dmendoza/logs/files/logs/logT1.txt.gz");

    //variables trackes across all files
    int numTrimmed = 0;
    int adaptRemov = 0;
    int totalReads = 0;
    cout <<"*****SPLITING*****"<<endl;
    //ar1 can be in file
    //system("./split.sh arg1");

    // Open the directory
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(inPath.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = ent->d_name;
            if (filename.length() > 6 && filename.substr(filename.length() - 6) == ".fastq") {
                fastqFiles.push_back(inPath + '/' + filename);
                outFiles.push_back(outPath + '/' + filename);
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Unable to open directory: " << inPath << std::endl;
    }

    //exit(0);

    cout << "Attempting parallel trim" <<endl;
    #pragma omp parallel for
    for(int i =0; i < fastqFiles.size(); i++){
        trim(fastqFiles[i], outFiles[i], logFile, numTrimmed, adaptRemov, totalReads);
    }
    cout << "Success! Combining Temp Files" <<endl;
    //combineGzFiles(outFile, tempFilesPath);



    //end program
    cout << "-----DONE-----" << endl;
    cout << "-----Summery-----" << endl;
    cout << "Adapters removed: " << adaptRemov << endl;
    cout << "Read trimmed: " << numTrimmed << endl;
    cout << "Total reads: " << totalReads;
    cout << totalReads-numTrimmed <<endl;
    //Summery
    return(1);
}
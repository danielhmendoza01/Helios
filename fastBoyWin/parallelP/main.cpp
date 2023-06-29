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
using namespace std;

string inFile = "/scratch/dmendoza/Sample_COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY/COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY_TTGGACGT_L004_R1_001.fastq.gz";
string outFile = "/scratch/dmendoza/logs/files/fastqFiles/outputT1.fastq.gz";

vector<string> tempFilesPath;
vector<string> tempFilesOut;
int main(){
    gzFile logFile = createGzFile("/scratch/dmendoza/logs/files/logs/logT1.txt.gz");

    //variables trackes across all files
    int numTrimmed = 0;
    int adaptRemov = 0;
    int totalReads = 0;
    cout <<"*****SPLITING*****"<<endl;
    fileSplit(inFile, tempFilesPath, tempFilesOut);

    cout << "Attempting parallel trim" <<endl;
    #pragma omp parallel for
    for(int i =0; i < tempFilesPath.size(); i++){
        trim(tempFilesPath[i], tempFilesOut[i], logFile, numTrimmed, adaptRemov, totalReads);
    }
    cout << "Success! Combining Temp Files" <<endl;
    combineGzFiles(outFile, tempFilesPath);



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
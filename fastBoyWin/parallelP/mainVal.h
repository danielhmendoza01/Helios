#include "fileReader.h"
#include "window.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <zlib.h>
using namespace std;
void trim(string inFile, string outFile, gzFile logFile, int& numTrimmed, int& adaptRemov, int& totalReads){
    #pragma omp critial
    {
    cout << "-----Program TRIM Starting-----\nWith Thread: " << omp_get_thread_num << endl;
    }
    // Needed variables
    unordered_set<std::string> identifierLines;
    const unsigned long GIGABYTE = 1024 * 1024 * 1024;
    char rawBuffer[1048576*100];
    string buffer;
    vector<char> lines[4];
    vector<int> numericLine;
    int lineIndex = 0;
    int relativeReads = 0;
    int trackMin = 0;
    int trackOccurrence = 60;
    vector<int> cutIndexes;
    // Variables set by user
    int baseSize = 50; //flex
    const char* filename = inFile.data();

    //string patternString = " ";
    //std::vector<char> pattern(patternString.begin(), patternString.end());
    vector<char> pattern {'C','G','T','G','T','G','C'}; //flex
    int patternSize = pattern.size();
    
    //File that needes to be read
    gzFile file = gzopen(filename, "r");
    if (file == nullptr) {
        cerr << "Error opening file: " << filename << endl;
        exit(0);
    }

    /*********************/
    //output will be the trim fastq.gz file
    gzFile outputFile = createGzFile(outFile);
    /********************/
    //gzFile logFile = createGzFile("/scratch/dmendoza/logs/files/logs/logT1.txt.gz");
    /*******************/

    //While loop to read all information of read file
    auto start = std::chrono::steady_clock::now();

    while (true) {
        int bytesRead = gzread(file, rawBuffer, sizeof(rawBuffer));
        if (bytesRead <= 0) {
            break;
        }

        buffer.append(rawBuffer, bytesRead);

        size_t pos;
        //while loop that finds a new line 
        while ((pos = buffer.find('\n')) != string::npos) {
            //stores the line from the start position to the next line
            //this is than stored in the lines vector where line number correlates to the vector index
            //this data is than removed from the buffer as it is no longer needed to move on for the next line
            string lineData = buffer.substr(0, pos);
            lines[lineIndex].insert(lines[lineIndex].end(), lineData.begin(), lineData.end());
            buffer.erase(0, pos + 1);
            /*********/
            if (lineIndex == 0){
                string line(lines[0].begin(), lines[0].end());
                try
                {
                    checkDuplicates(identifierLines, line);
                }
                catch(const std::exception& e)
                {
                    cout << "*****Failed Running Duplicate Function******" << endl;
                    std::cerr << e.what() << '\n';
                }
            }
            if (lineIndex == 2){
                for(int i = 0; i<lines[2].size(); i++){
                    if (lines[2][i] != '+'){
                        cout << "*****LINKER ERROR*****" << endl;
                        exit(0);
                    }
                }
            }
            /*********/
            //At lineIndex 3 the ASCII line occurs 
            if (lineIndex == 3) {
                //boyer
                search(lines[1], pattern, lines[3], adaptRemov);
                /******/
                if (lines[1].size() < baseSize || lines[3].size() < baseSize)
                {
                    cout << "********BOYER UNDER MIN LENGTH ERROR*******\n"<< endl;
                    exit(0);
                }
                if(lines[1].size() != lines[3].size()){
                    cout << "********BOYER SAME LENGTH ERROR*******\n" << endl;
                    exit(0);
                }
                /*******/

                //window
                eraseCutoff(lines[1], lines[3], numericLine, numTrimmed, logFile);
                /******/
                if (lines[1].size() < baseSize || lines[3].size() < baseSize)
                {
                    cout << "********WINDOW UNDER MIN LENGTH ERROR*******\n" << endl;
                    exit(0);
                }
                if(lines[1].size() != lines[3].size()){
                    cout << "********WINDOW SAME LENGTH ERROR*******\n" << endl;
                    exit(0);
                }
                /*******/
                //write the 4 lines to the out file
                for (int i = 0; i < 4; ++i) {
                    writeDataToGzFile(outputFile, lines[i]);
                    lines[i].clear();
                }
                numericLine.clear();
                totalReads++;
                relativeReads++;
            }
            //Cycle through the four lines of the fastq format
            //After processing the fourth line (index 3) wraps back to the first line (index 0)
            lineIndex = (lineIndex + 1) % 4;
        }
        //This statment is to check that the proccess is not in the middle of a section (four lines)
        //linesIndex should be 0 if at the start of a section
        //Also checks that the buffer still has data remaining
        //This remaining data is than added
        if (lineIndex > 0 && !buffer.empty()) {
            string lineData = buffer;
            lines[lineIndex].insert(lines[lineIndex].end(), lineData.begin(), lineData.end());
            buffer.clear();
        }
        auto check = std::chrono::steady_clock::now();
        auto timeElapsed = std::chrono::duration_cast<std::chrono::seconds>(check - start).count();
        if(timeElapsed / trackOccurrence != trackMin){
            cout << "Time: " << timeElapsed/trackOccurrence << " Min" << setw(18);
            cout << "Total Reads: " << totalReads<< setw(18);
            cout << "Reads/Min: " << relativeReads <<"\n" << endl;
            relativeReads = 0;
            trackMin++;
        }
    }
    //close files
    cout << "-----Ending-----" << endl;
    cout << "Closing file..." << endl;
    gzclose(file);
}
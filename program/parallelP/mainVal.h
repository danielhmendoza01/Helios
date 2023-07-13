#include "window.h"
#include "time.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <zlib.h>
#include <omp.h>
#include <cstdio>
using namespace std;
void trim(string inFile, string outFile, gzFile logFile, int& numTrimmed, int& adaptRemov, long& totalReads, string patternString, int windowSize, int baseSize, int threshold){
    #pragma omp critical
    {
    cout << "Trimming starting with thread: " << omp_get_thread_num() << endl;
    }
    // Needed variables
    Timer inTimer;
    Timer boyerTimer;
    Timer windowTimer;
    Timer writeTimer;
    unordered_set<std::string> identifierLines;
    char rawBuffer[1024];
    string buffer;
    vector<char> lines[4];
    vector<int> numericLine;
    int lineIndex = 0;
    int relativeReads = 0;
    int trackTime = 0;
    int trackOccurrence = 30;
    vector<int> cutIndexes;
    // Variables set by user
    int baseSize = 50; //flex

    vector<char> pattern(patternString.begin(), patternString.end());
    //vector<char> pattern {'C','G','T','G','T','G','C'}; //flex
    int patternSize = pattern.size();
    
    //File that needes to be read
    ifstream file(inFile);
    if (!file.is_open()) {
        cerr << "Error opening file: " << inFile << endl;
        exit(0);
    }
    //output will be the trim fastq file
    ofstream outputFile(outFile);
    //While loop to read all information of read file
    auto start = std::chrono::steady_clock::now();
    while (true) {
        // Read file with buffer size
        inTimer.start();
        file.read(rawBuffer, sizeof(rawBuffer));
        int bytesRead = file.gcount();
        if (bytesRead <= 0) {
            break;
        }
        buffer.append(rawBuffer, bytesRead);
        inTimer.stop();

        size_t pos;
        //while loop that finds a new line 
        while ((pos = buffer.find('\n')) != string::npos) {
            //stores the line from the start position to the next line
            //this is than stored in the lines vector where line number correlates to the vector index
            //this data is than removed from the buffer as it is no longer needed to move on for the next line
            string lineData = buffer.substr(0, pos);
            lines[lineIndex].insert(lines[lineIndex].end(), lineData.begin(), lineData.end());
            buffer.erase(0, pos + 1);

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

            // At lineIndex 3 the ASCII line occurs(4th line)
            if (lineIndex == 3) {
                //boyer
                boyerTimer.start();
                search(lines[1], pattern, lines[3], adaptRemov);
                if (lines[1].size() < baseSize || lines[3].size() < baseSize)
                {
                    cout << "********BOYER UNDER MIN LENGTH ERROR*******\n"<< endl;
                    exit(0);
                }
                if(lines[1].size() != lines[3].size()){
                    cout << "********BOYER SAME LENGTH ERROR*******\n" << endl;
                    exit(0);
                }
                boyerTimer.stop();

                //window
                windowTimer.start();
                eraseCutoff(lines[1], lines[3], numericLine, numTrimmed, logFile, windowSize, baseSize, threshold);
                if (lines[1].size() < baseSize || lines[3].size() < baseSize)
                {
                    cout << "********WINDOW UNDER MIN LENGTH ERROR*******\n" << endl;
                    exit(0);
                }
                if(lines[1].size() != lines[3].size()){
                    cout << "********WINDOW SAME LENGTH ERROR*******\n" << endl;
                    exit(0);
                }
                windowTimer.stop();

                writeTimer.start();
                //write the 4 lines to the out file
                for (int i = 0; i < 4; ++i) {
                    outputFile.write(&lines[i][0], lines[i].size());
                    outputFile << "\n";
                    lines[i].clear();
                }
                numericLine.clear();
                writeTimer.stop();
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
        if(timeElapsed / trackOccurrence != trackTime){
            #pragma omp critical
            {
            cout << "Thread: " << omp_get_thread_num() << setw(10);
            cout << "Time: " << timeElapsed << " Sec" << setw(18);
            cout << "Total Reads: " << totalReads<< setw(22);
            cout << "Reads per 30 sec: " << relativeReads <<"\n" << endl;
            }
            relativeReads = 0;
            trackTime++;
        }
    }
    //close files
    file.close();
    outputFile.close();
    #pragma omp critical
    {
    cout << "Thread " << omp_get_thread_num() << endl;
    cout << "-----Ending File: " << inFile<< "-----" << endl;
    cout << "Total Reads: " << totalReads << endl;
    cout << "Total read ";
    inTimer.printElapsedTime();
    cout << "Adapter removal ";
    boyerTimer.printElapsedTime();
    cout << "Quality trimming ";
    windowTimer.printElapsedTime();
    cout << "File writing ";
    writeTimer.printElapsedTime();
    cout << "Closed: " << inFile << endl;
    remove(inFile.c_str());
    cout << "Deleted: " << inFile << endl;
    }
    //cout << "Compressing: " << outFile << endl;
    //string pCompress = "gzip " + outFile;
    //system(pCompress.c_str());
}
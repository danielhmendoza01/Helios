#include <chrono>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <zlib.h>
using namespace std;

//Function to create a GZ File that can be written
gzFile createGzFile(const std::string& filename) {
    gzFile file = gzopen(filename.c_str(), "wb");
    if (file == nullptr) {
        std::cerr << "Error opening " << filename << " for writing." << std::endl;
        exit(1);
    }
    return file;
}

//function to write to the gzFile
//It takes in a char vector and on a new line the data in the vector is added to file
void writeDataToGzFile(gzFile file, std::vector<char> data) {
    data.push_back('\n');
    gzwrite(file, data.data(), data.size());
}

int main() {
    const char* filename = "/scratch/dmendoza/Sample_COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY/COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY_TTGGACGT_L004_R1_001.fastq.gz";
    gzFile file = gzopen(filename, "r");
    if (file == nullptr) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }
    //output will be the trim fastq.gz file
    gzFile outputFile = createGzFile("outputTest.fastq.gz");
    /******/
    gzFile logFile = createGzFile("test.txt.gz");
    /******/
    char rawBuffer[1024];
    string buffer;
    vector<char> lines[4];
    int lineIndex = 0;
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
            //Avoid NonASCII Reads
            lines[lineIndex].insert(lines[lineIndex].end(), lineData.begin(), lineData.end());
            buffer.erase(0, pos + 1);
            //At lineIndex 3 the ASCII line occurs 
            if (lineIndex == 3) {
                //write the 4 lines to the out file
                for (int i = 0; i < 4; ++i) {
                    writeDataToGzFile(outputFile, lines[i]);
                    lines[i].clear();
                }
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
    }
    //close files
    cout << "-----Ending-----" << endl;
    cout << "Closing files..." << endl;
    gzclose(file);
    gzclose(outputFile);
    gzclose(logFile);
}
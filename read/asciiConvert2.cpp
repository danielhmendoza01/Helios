#include <iostream>
#include <fstream>
#include <zlib.h>
#include <cstring>
#include <vector>

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

//takes the avg of the window and returns the cutoff if condition is met
//returns -1 if no cutoff is needed
int windowSlide(std::vector<int> numLine){
    if (numLine.empty()) {
        return -1; // Handle empty vector case
    }
    
    int i = 0;
    int avg = 0;
    int threshold = 20;
    //Window set size
    int windowSize = 5;
    //While loop that occurs until the window reaches the end of the line
    while (i + windowSize <= numLine.size()){
        int last = i + windowSize;
        //For loop that sums all the numbers within the window
        for (int index = i; index < last; index++){
            avg += numLine[index];
        }
        //Get the average be deviding by the window size
        avg = avg / windowSize;
        //Check if the avgerage is less than the threshold
        if (avg < threshold){
            return i;
        }
        //next window and reset avg back to 0
        i++;
        avg = 0;
    }
    return -1;
}

int main() {
    //File that needes to be read
    const char* filename = "/scratch/dmendoza/Sample_COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY/COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY_TTGGACGT_L004_R1_001.fastq.gz";
    //output will be the trim fastq.gz file
    gzFile outputFile = createGzFile("/scratch/dmendoza/logs/output.fastq.gz");
    gzFile file = gzopen(filename, "r");
    if (file == nullptr) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    char rawBuffer[2000];
    std::string buffer;
    std::vector<char> lines[4];
    std::vector<int> numericLine;
    int lineIndex = 0;
    int baseSize = 50;

    //While loop to read all information of read file
    while (true) {
        int bytesRead = gzread(file, rawBuffer, sizeof(rawBuffer));
        if (bytesRead <= 0) {
            break;
        }

        buffer.append(rawBuffer, bytesRead);

        size_t pos;
        //while loop that finds a new line 
        while ((pos = buffer.find('\n')) != std::string::npos) {
            //stores the line from the start position to the next line
            //this is than stored in the lines vector where line number correlates to the vector index
            //this data is than removed from the buffer as it is no longer needed to move on for the next line
            std::string lineData = buffer.substr(0, pos);
            lines[lineIndex].insert(lines[lineIndex].end(), lineData.begin(), lineData.end());
            buffer.erase(0, pos + 1);
            //At lineIndex 3 the ASCII line occurs 
            if (lineIndex == 3) {
                for (char c : lines[3]) {
                    int numericalValue = static_cast<int>(c) - 33;
                    numericLine.push_back(numericalValue);
                }
                //Window slide function finds where the cutoff should be if needed 
                int cutOff = windowSlide(numericLine);
                //if the cut off is -1 than no cut off is needed
                //if the cut off is not -1 it holds the threshold index to cutoff
                if (cutOff != -1){
                    if(cutOff >= baseSize){
                        //erase the ASCII and Sequence line at the cutoff
                        lines[3].erase(lines[3].begin() + cutOff, lines[3].end());
                        lines[1].erase(lines[1].begin() + cutOff, lines[1].end());
                    }
                }
                //write the 4 lines to the out file
                for (int i = 0; i < 4; ++i) {
                    writeDataToGzFile(outputFile, lines[i]);
                    lines[i].clear();
                }
                numericLine.clear();
            }
            //Cycle through the four lines of the fastq format
            //After processing the fourth line (index 3) wraps back to the first line (index 0)
            lineIndex = (lineIndex + 1) % 4;
        }
        //
        //This statment is to check that the proccess is not in the middle of a section (four lines)
        //linesIndex should be 0 if at the start of a section
        //Also checks that the buffer still has data remaining
        //This remaining data is than added
        if (lineIndex > 0 && !buffer.empty()) {
            std::string lineData = buffer;
            lines[lineIndex].insert(lines[lineIndex].end(), lineData.begin(), lineData.end());
            buffer.clear();
        }
    }
    //close files
    gzclose(file);
    gzclose(outputFile);
    //end program
    return 0;
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <zlib.h>
using namespace std;

bool concatenateAndGzipFiles(const std::vector<std::string>& fileNames, const std::string& outputFileName) {
    gzFile outFileZ = gzopen(outputFileName.c_str(), "wb");
    if (!outFileZ) {
        std::cerr << "Cannot open output file: " << outputFileName << std::endl;
        return false;
    }

    char buffer[1024];
    for (const auto& fileName : fileNames) {
        std::ifstream inFile(fileName, std::ios::binary);
        if (!inFile) {
            std::cerr << "Cannot open input file: " << fileName << std::endl;
            gzclose(outFileZ);
            return false;
        }

        while (inFile.read(buffer, sizeof(buffer))) {
            if (gzwrite(outFileZ, buffer, inFile.gcount()) != inFile.gcount()) {
                std::cerr << "Failed to write to the gz file" << std::endl;
                gzclose(outFileZ);
                return false;
            }
        }
    }

    gzclose(outFileZ);
    return true;
}

int main() {
    std::vector<std::string> fileNames = {"/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split01.fastq","/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split02.fastq","/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split03.fastq","/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split04.fastq","/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split05.fastq","/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split06.fastq","/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split07.fastq","/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split08.fastq","/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split09.fastq","/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split10.fastq","/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split11.fastq","/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split12.fastq","/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split13.fastq","/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split14.fastq"};  // TODO: your file list
    std::string outputFileName = "output.fastq.gz";

    if (concatenateAndGzipFiles(fileNames, outputFileName)) {
        std::cout << "Success!" << std::endl;
    } else {
        std::cout << "Failure!" << std::endl;
    }

    return 0;
}

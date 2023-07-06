#include <iostream>
#include <fstream>
#include <vector>
#include <zlib.h>

gzFile createGzFile(const std::string& filename) {
    gzFile file = gzopen(filename.c_str(), "wb");
    if (file == nullptr) {
        std::cerr << "Error opening " << filename << " for writing." << std::endl;
        exit(1);
    }
    return file;
}
void writeDataToGzFile(gzFile file, std::vector<char> data) {
    data.push_back('\n');
    gzwrite(file, data.data(), data.size());
}
int main() {
    const char* filename = "/scratch/dmendoza/Sample_COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY/COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY_TTGGACGT_L004_R1_001.fastq.gz";
    gzFile file = gzopen(filename, "r");
    gzFile outputFile = createGzFile("out.fastq.gz");
    if (!file) {
        std::cerr << "Error opening input file.\n";
        return 1;
    }

    if (!outputFile) {
        std::cerr << "Error opening output file.\n";
        return 1;
    }

    std::vector<char> buffer(1048576*100); // 1MB buffer
    int bytesRead = gzread(file, buffer.data(), buffer.size());
    if (bytesRead < 0) {
        std::cerr << "Error reading input file.\n";
        return 1;
    }
    buffer.resize(bytesRead); // Resize the buffer to the actual number of bytes read

    writeDataToGzFile(outputFile,buffer);
    gzclose(file);
    gzclose(outputFile);
    return 0;
}

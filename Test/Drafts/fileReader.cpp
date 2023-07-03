#include <iostream>
#include <fstream>
#include <cstring>
#include <zlib.h>
#include <vector>
using namespace std;

//function to write to the gzFile
//It takes in a char vector and on a new line the data in the vector is added to file
void writeDataToGzFile(gzFile file, std::vector<char> data) {
    data.push_back('\n');
    gzwrite(file, data.data(), data.size());
}
void writeBufferToGzFile(gzFile file, std::vector<char> data){
    gzwrite(file, data.data(), data.size());
}
void combineGzFiles(const std::string& output_file, const std::vector<std::string>& gz_files) {
    std::ofstream output(output_file, std::ios::binary);
    if (!output) {
        std::cerr << "Failed to open output file: " << output_file << std::endl;
        return;
    }

    for (const auto& gz_file : gz_files) {
        gzFile input = gzopen(gz_file.c_str(), "rb");
        if (!input) {
            std::cerr << "Failed to open input file: " << gz_file << std::endl;
            return;
        }

        char buffer[1024];
        int num_read = 0;
        while ((num_read = gzread(input, buffer, sizeof(buffer))) > 0) {
            output.write(buffer, num_read);
        }

        gzclose(input);
    }

    output.close();
}





int main() {
    const char *filename = "/scratch/dmendoza/logs/files/fastqFiles/tTestOut/bigfile-split14.fastq";

    gzFile file = gzopen(filename, "r");
    if (file == nullptr) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    char buffer[1024];
    int bytesRead;
    while ((bytesRead = gzread(file, buffer, sizeof(buffer))) > 0) {
        // Process the data in the buffer
        // bytesRead can be used to track the progress
        // Example: Print the buffer
        std::cout.write(buffer, bytesRead);
        //break;
    }
    cout << endl;
    cout << "______________________________________________________________________________________________________________________________________________________" << endl;
    gzclose(file);
    return 0;
}

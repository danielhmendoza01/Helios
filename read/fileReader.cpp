#include <iostream>
#include <fstream>
#include <zlib.h>
using namespace std;

int main() {
    const char *filename = "/scratch/dmendoza/Helios_Project/fastBoyWin/files/logs/output.fastq.gz";

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
    }

    gzclose(file);
    return 0;
}

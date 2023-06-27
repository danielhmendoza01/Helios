#include <iostream>
#include <fstream>
#include <zlib.h>

int main() {
    const char* filename = "/scratch/dmendoza/logs/files/fastqFiles/tTest/temp0.fastq.gz";
    const int numLines = 4;
    const int bufferSize = 1024;

    gzFile file = gzopen(filename, "rb");
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    gzseek(file, -bufferSize * numLines, SEEK_END);  // Jump to the position before the last 4 lines

    char buffer[bufferSize];
    std::string lines[numLines];

    int bytesRead;
    int lineCount = 0;

    while ((bytesRead = gzread(file, buffer, bufferSize - 1)) > 0) {
        buffer[bytesRead] = '\0';
        char* pos = buffer + bytesRead - 1;

        while (pos >= buffer) {
            if (*pos == '\n') {
                if (lineCount >= numLines)
                    break;

                lines[lineCount] = std::string(pos + 1) + lines[lineCount];
                lineCount++;
            }
            pos--;
        }

        if (lineCount >= numLines)
            break;

        gzseek(file, -bufferSize * numLines, SEEK_CUR);
    }

    gzclose(file);

    for (int i = 0; i < lineCount; i++) {
        std::cout << lines[i] << std::endl;
    }

    return 0;
}

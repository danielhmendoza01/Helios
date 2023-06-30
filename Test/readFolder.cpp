#include <iostream>
#include <dirent.h>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

int main() {
    std::string inPath = "/scratch/dmendoza/Sample_COVA_8059_1_OV_Whole_C1_KHWGS_K20519_HKVHCDSXY/filesSplit";  // Replace with the path to your folder
    vector<string> fastqFiles;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(inPath.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = ent->d_name;
            if (filename.length() > 6 && filename.substr(filename.length() - 6) == ".fastq") {
                fastqFiles.push_back(inPath + '/' + filename);
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Unable to open directory: " << inPath << std::endl;
    }
    for(int i; i< fastqFiles.size(); i++){
        cout << fastqFiles[i] << endl;
    }

    return 0;
}

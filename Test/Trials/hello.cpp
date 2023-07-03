#include <iostream>
using namespace std;
int main(int argc, char* argv[]) {
    // Access the file paths as command-line arguments
    std::string file1 = argv[1];
    std::string file2 = argv[2];
    std::string file3 = argv[3];

    // Rest of your C++ code
    cout << file1 << file2 << file3 << endl;
    return 0;
}

#include <iostream>
#include<vector>
using namespace std;
int main() {
    vector<char> line = {'g', 'c', 'a', 'a', 't', 'g', 'c', 'c', 't', 'a','t','g','t','g','a','a'};
    vector<char> pattern = {'t','a','t','g','t','g'};
    line.erase(line.begin() + 8, line.begin() + 8 + pattern.size());
    for(int i; i < line.size(); i++){
       cout << line[i] << " ";
    }
    cout << endl;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
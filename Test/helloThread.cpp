#include <iostream>
#include <cstring>
#include <thread>
#include <vector>
#include <random>
using namespace std;

string first = "S";
string second = "1";

string writeData(int num, string letters, char sym) {
    cout << "INSIDE" << sym << endl;
    for (int i = 0; i < num; i++) {

        // Obtain a random seed based on the current time
        unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
        std::default_random_engine generator(seed);

        // Generate a random duration between 1 and 5 seconds
        std::uniform_int_distribution<int> distribution(1, 2);
        int randomSeconds = distribution(generator);

        // Pause the program for the random duration
        std::this_thread::sleep_for(std::chrono::seconds(randomSeconds));

        // Continue with the rest of the program
        std::cout << "Pause complete. Continuing program execution." << std::endl;

        cout << sym << " ";
        letters = letters + 'A';
    }
    cout << endl;
    return letters;
}

int main() {
    string result1;
    string result2;

    thread t1([&result1]() {
        result1 = writeData(10000, first, '$');
    });
    cout << "_______" << endl;
    thread t2([&result2]() {
        result2 = writeData(10000, second, '&');
    });

    //string a = writeData(10, first, '|');

    //cout << first << endl;
    //cout << a << endl;

    t1.join();
    t2.join();

    //cout << "Result from thread 1: " << result1 << endl;
    //cout << "Result from thread 2: " << result2 << endl;

    return 0;
}

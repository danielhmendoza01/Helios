#include <iostream>
#include <chrono>
using namespace std;
class Timer
{
private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
    int64_t totalTime = 0;

public:
    Timer() {}

    void start()
    {
        startTime = std::chrono::high_resolution_clock::now();
    }

    void stop()
    {
        endTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        totalTime = totalTime + elapsedTime.count();
    }

    void printElapsedTime()
    {
        //auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        std::cout << "elapsed time: " << totalTime/1000000 << " seconds." << std::endl;
    }
};


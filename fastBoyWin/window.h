#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <zlib.h>
using namespace std;

//takes the avg of the window and returns the cutoff if condition is met
//returns -1 if no cutoff is needed
int windowSlide(vector<int> numLine){
    if (numLine.empty()) {
        return -1; // Handle empty vector case
    }
    
    int i = 0;
    int avg = 0;
    int threshold = 20;
    //Window set size
    int windowSize = 5;
    //While loop that occurs until the window reaches the end of the line
    while (i + windowSize <= numLine.size()){
        int last = i + windowSize;
        //For loop that sums all the numbers within the window
        for (int index = i; index < last; index++){
            avg += numLine[index];
        }
        //Get the average be deviding by the window size
        avg = avg / windowSize;
        //Check if the avgerage is less than the threshold
        if (avg < threshold){
            return i;
        }
        //next window and reset avg back to 0
        i++;
        avg = 0;
    }
    return -1;
}

void eraseCutoff(vector<char>& line, vector<char>& score, vector<int>& numericLine){
    int baseSize = 50;
    for (char c : score) {
        int numericalValue = static_cast<int>(c) - 33;
        numericLine.push_back(numericalValue);
    }
    //Window slide function finds where the cutoff should be if needed 
    int cutOff = windowSlide(numericLine);
    // if the cut off is not -1 it holds the threshold index to cutoff
    if (cutOff != -1 && cutOff >= baseSize){
        // erase the ASCII and Sequence line at the cutoff
        score.erase(score.begin() + cutOff, score.end());
        line.erase(line.begin() + cutOff, line.end());
    }
    
}
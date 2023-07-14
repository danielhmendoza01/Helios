#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <zlib.h>
#include <omp.h>
#include "boyer.h"
using namespace std;

//takes the avg of the window and returns the cutoff if condition is met
//returns -1 if no cutoff is needed
int windowSlide(vector<int> numLine, int windowSize, int threshold){
    if (numLine.empty()) {
        return -1; // Handle empty vector case
    }
    
    int i = 0;
    int avg = 0;
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
int windowSlideReverse(vector<int> numLine, int windowSize, int threshold){
    if (numLine.empty()) {
        return -1; // Handle empty vector case
    }
    
    int i = numLine.size() - windowSize; // start from the right
    int avg = 0;
    //While loop that occurs until the window reaches the beginning of the line
    while (i >= 0){
        int first = i;
        int last = i + windowSize;
        //For loop that sums all the numbers within the window
        for (int index = first; index < last; index++){
            avg += numLine[index];
        }
        //Get the average by dividing by the window size
        avg = avg / windowSize;
        //Check if the average is greater than the threshold
        if (avg > threshold){
            return i + windowSize; // return the right bound of the window
        }
        //next window and reset avg back to 0
        i--;
        avg = 0;
    }
    return -1;
}


void eraseCutoff(vector<char>& line, vector<char>& score, vector<int>& numericLine, int& numTrimmed, gzFile logFile, int windowSize, int baseSize, int threshold, bool reverseQTrim, vector<char> id){
    for (char c : score) {
        int numericalValue = static_cast<int>(c) - 33;
        numericLine.push_back(numericalValue);
    }
    //Window slide function finds where the cutoff should be if needed
    int cutOff;
    if(reverseQTrim){
        cutOff = windowSlideReverse(numericLine, windowSize, threshold);
    }
    else{
        cutOff = windowSlide(numericLine, windowSize, threshold);
    } 
    
    // if the cut off is not -1 it holds the threshold index to cutoff
    if (cutOff != -1 && cutOff >= baseSize){
        // erase the ASCII and Sequence line at the cutoff
        score.erase(score.begin() + cutOff, score.end());
        line.erase(line.begin() + cutOff, line.end());
        numTrimmed++;
    }
    if (cutOff < baseSize && cutOff != -1)
    {
        string idLine(id.begin(), id.end());
        #pragma omp critical
        {
        gzWriteStringToGzFile(logFile, "***WARNING*****\n" + idLine + "\nNot trimmed: Below min cut off\n");
        }
    }
}
#pragma once
#include<bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <zlib.h>
#include <cstring>
#include <vector>
#include "validate.h"
using namespace std;

// 256 is needed for array memory based on ASCII char
#define NO_OF_CHARS 256

// The preprocessing function for Boyer Moore's (Precompute the table)
// Array will be filled with the roght most index of occurrence of each character in the pattern 
// -1 if the character does not appear in the pattern
// This information is then used in the Boyer-Moore algorithm to perform efficient shifts when a character mismatch occurs
// Determine the amount to shift the pattern when a mismatch occurs.
void badCharHeuristic(vector<char> pat, int size, int badchar[NO_OF_CHARS])
{
    int i;
    //set array to all -1
    for (i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;

    for (i = 0; i < size; i++)
        //Asiagn Array at ASCII to the right most index
        badchar[(int) pat[i]] = i;
}

//Good Suffix
// A function to fill shift for the given pattern
// Calculates the necessary shift values for each position in the pattern.
void preprocess_strong_suffix(vector<char> pattern, int size, vector<int>& shift)
{
    // bpos will be a vector of pattern size + 1 and initialized to 0
    // bpos will be used to store border positions of a pattern 
    // (a proper suffix and prefix)
    vector<int> bpos(size + 1, 0);
    int i = size, j = size + 1;
    bpos[i] = j;
    while (i > 0)
    {
        //shift pattern until next occurence is found
        // check if char at [i-1] of the pattern does not equal [j-1] of the pattern *** why
        while (j <= size && pattern[i - 1] != pattern[j - 1])
        {
            //shift pattern ***explain
            if (shift[j] == 0)
                shift[j] = j - i;
            //update position for next border
            j = bpos[j];
        }
        // Border is found
        i--;
        j--;
        bpos[i] = j;
    }
    //Case 2
    //prefix to suffix
    j = bpos[0];
    for (i = 0; i <= size; i++)
    {
        // border position of first character pf pattern to where shift is 0
        if (shift[i] == 0)
            shift[i] = j;
        //if suffix is shorter border position 
        //set to next widest border
        if (i == j)
            j = bpos[j];
    }
}

void search(vector<char>& line, vector<char>& pattern, vector<char>& score, int& adaptRemov)
{
    int lineSize = line.size();
    int patternSize = pattern.size();
    vector<int> shift(patternSize + 1, 0);
    int badchar[NO_OF_CHARS];

    validate_vector(line);

    preprocess_strong_suffix(pattern, patternSize, shift);
    badCharHeuristic(pattern, patternSize, badchar);

    vector<int> positions;
    int shiftIndex = 0;

    while (shiftIndex <= (lineSize - patternSize))
    {
        int lastPIndex = patternSize - 1;
        while (lastPIndex >= 0 && pattern[lastPIndex] == line[shiftIndex + lastPIndex]){
            lastPIndex--;
        }
        if (lastPIndex < 0)
        {
            positions.push_back(shiftIndex);
            shiftIndex += patternSize;
            
        }
        else
            shiftIndex += max(shift[lastPIndex + 1], lastPIndex - badchar[line[shiftIndex + lastPIndex]]);
    }

    if (!positions.empty())
    {
        for (int i = positions.size()-1; i >= 0; i--)
        {
            /***********/
            vector<char> subLine(line.begin() + positions[i], line.begin() + positions[i] + patternSize);
            for (int j = 0; j < pattern.size(); j++) {
                if (pattern[j] != subLine[j]) {                    
                    cout << "****ADAPTER ERROR*****\n" << endl;
                    exit(0);
                }
            }
            /***********/
            //how to erase
            line.erase(line.begin() + positions[i], line.begin() + positions[i] + patternSize);
            score.erase(score.begin() + positions[i], score.begin() + positions[i] + patternSize);
            adaptRemov++;
        }
    }
}

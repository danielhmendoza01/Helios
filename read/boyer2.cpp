#include<bits/stdc++.h>
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

//Return all indexes where the pattern is found or an empty vector if none is found
vector<int> search(vector<char> line, vector<char> pattern)
{
    int patternSize = pattern.size();
    int lineSize = line.size();
    //***size+1?
    vector<int> shift(patternSize + 1, 0);
    int badchar[NO_OF_CHARS];

    // Passing by reference
    preprocess_strong_suffix(pattern, patternSize, shift);
    badCharHeuristic(pattern, patternSize, badchar);
    int count = 0;
    // Vector to hold all starting positions of the pattern
    vector<int> positions; 

    int shiftIndex = 0;
    //search line
    while(shiftIndex <= (lineSize - patternSize))
    {
        //Right most index of the pattern
        int lastPIndex = patternSize - 1;
        //while character of the pattern right to left match at this shift
        while(lastPIndex >= 0 && pattern[lastPIndex] == line[shiftIndex + lastPIndex])
            lastPIndex--;

        // If the pattern is presesent when last index will become -1
        if (lastPIndex < 0)
        {
            // Pattern found
            // Push the position to the vector
            positions.push_back(shiftIndex); 

            // After a match is found, shift pattern to align with the next character of the line to find another match
            if(shiftIndex + patternSize < lineSize)
                shiftIndex += patternSize - badchar[line[shiftIndex + patternSize]];
            else
                shiftIndex += 1;
        }
        //When there is not a complete match the shift index is updated
        //
        else
            //check shift array at last match
            /*std::cout << "shiftI " << shiftIndex << "righP " << lastPIndex << std::endl; 
            std::cout << "THIS char: " << (line[shiftIndex + lastPIndex]) << std::endl;
            std::cout << "Bad char index " << (badchar[line[shiftIndex + lastPIndex]]) << std::endl;
            std::cout << "Bad char sub " << (lastPIndex -(badchar[line[shiftIndex + lastPIndex]])) << std::endl;
            std::cout << "Shift Calc: " << shift[lastPIndex + 1] << std::endl;*/
            shiftIndex += max(shift[lastPIndex + 1], lastPIndex - badchar[line[shiftIndex + lastPIndex]]);
    }

    return positions; // return all positions
}

int main()
{
    vector<char> line = {'g', 'c', 'a', 'a', 't', 'g', 'c', 'c', 't', 'a','t','g','t','g','a','c','c','t','a','t','g','t','g'};
    vector<char> pattern = {'t','a','t','g','t','g'};
    vector<int> results = search(line, pattern);
    if(results.empty())
        cout << "Pattern not found" << endl;
    else {
        cout << "Pattern found at indexes: ";
        for(int i : results)
            cout << i << " ";
        cout << endl;
    }
    return 0;
}

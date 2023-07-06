#include<bits/stdc++.h>
using namespace std;

#define NO_OF_CHARS 256

// The preprocessing function for Boyer Moore's
// bad character heuristic
// Array will be filled with the last index of occurrence of each character in the pattern string 
// (or -1 if the character does not appear in the pattern)
// This information is then used in the Boyer-Moore algorithm to perform efficient shifts when a character mismatch occurs.
void badCharHeuristic(vector<char> str, int size, int badchar[NO_OF_CHARS])
{
    int i;

    for (i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;

    for (i = 0; i < size; i++)
        badchar[(int) str[i]] = i;
}

//Good Suffix
// A function to fill shift for the given pattern
// Calculates the necessary shift values for each position in the pattern.
void preprocess_strong_suffix(vector<char> pattern, vector<int>& shift)
{
    int patternSize = pattern.size();
    // bpos will be a vector of pattern size + 1 and initialized to 0
    // bpos will be used to store border positions of a pattern 
    // (a proper suffix and prefix)
    vector<int> bpos(patternSize + 1, 0);
    int i = patternSize, j = patternSize + 1;
    bpos[i] = j;
    while (i > 0)
    {
        while (j <= patternSize && pattern[i - 1] != pattern[j - 1])
        {
            if (shift[j] == 0)
                shift[j] = j - i;
            j = bpos[j];
        }
        i--;
        j--;
        bpos[i] = j;
    }

    j = bpos[0];
    for (i = 0; i <= patternSize; i++)
    {
        if (shift[i] == 0)
            shift[i] = j;
        if (i == j)
            j = bpos[j];
    }
}

// Returns the index of first occurrence of pattern in line, or -1 if pattern is not present
int search(vector<char> line, vector<char> pattern)
{
    int patternSize = pattern.size();
    int lineSize = line.size();

    vector<int> shift(patternSize + 1, 0);
    int badchar[NO_OF_CHARS];

    // Passing by reference
    preprocess_strong_suffix(pattern, shift);
    badCharHeuristic(pattern, patternSize, badchar);

    int shiftIndex = 0;
    //Search line
    while(shiftIndex <= (lineSize - patternSize))
    {
        int lastPIndex = patternSize - 1;
        while(lastPIndex >= 0 && pattern[lastPIndex] == line[shiftIndex + lastPIndex])
            lastPIndex--;

        if (lastPIndex < 0)
            //Pattern found
            //Return the index
            return shiftIndex;
        //When there is not a complete match the shift index is updated
        else
            shiftIndex += max(shift[lastPIndex + 1], lastPIndex - badchar[line[shiftIndex + lastPIndex]]);
    }
    // pattern not found
    return -1; 
}

int main()
{
    vector<char> line = {'g', 'c', 'a', 'a', 't', 'g', 'c', 'c', 't', 'a','t','g','t','g','a','c','c'};
    vector<char> pattern = {'t','a','t','g','t','g'};
    int result = search(line, pattern);
    if(result == -1)
        cout << "Pattern not found" << endl;
    else
        cout << "Pattern found at index " << result << endl;
    return 0;
}

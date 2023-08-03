/*
This is a program the calculates the Maximum number of H-H contacts for an n-length prototein. It does it
sequentially by generating every single possible walk and scoring it.

@author: Owen Sheed and Joel Hollingsworth
*/
#include <iostream>
#include <string.h>
#include <math.h>
using namespace std;

// directions
#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

unsigned long long rdtsc() {
   unsigned hi, lo;
   __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
   return ((unsigned long long) lo) | (((unsigned long long) hi) << 32);
}

// turns base 10 label into base 4, creating every possible walk
void labelToWalk(int label,int *walk, int r) {
    for (int p = r-2; p >= 0; p--) {
        walk[p] = label / (int) pow(4, p);
        label = label % (int) pow(4, p);

    }
}

// Displays the walk in N,E,W, and S notation
void displayWalk(int label, int *walk, int r) {
    cout << label << ": ";
    for (int i = r-2; i >= 0; i--) {
        cout << walk[i];
    }
    cout << " ";
    for (int i = r-2; i >= 0; i--) {
        if (walk[i] == NORTH) cout << "N";
        if (walk[i] == SOUTH) cout << "S";
        if (walk[i] == EAST)  cout << "E";
        if (walk[i] == WEST)  cout << "W";
    }
    cout << endl;
}


// Scores each walk
int score(char *prototein, int *walk, int r) {
    // creating size measurement that is larger than 2 times our prototein. This is so we can iterate through the array without worrying about index out of bounds.
    int size = (2 * r) + 1;

    // Creating the array
    char graph[size][size];

    // Initializing the array
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            graph[i][j] = '.';
        }
    }

    int row = r;
    int col = r;

    // putting the first part of the prototein at the center
    graph[row][col] = prototein[0];

    for (int i = 1; i < r; i++) {
        // Navigating through the array to create the walk
        if (walk[i-1] == NORTH) row--;
        if (walk[i-1] == SOUTH) row++;
        if (walk[i-1] == EAST ) col++;
        if (walk[i-1] == WEST ) col--;
        
        // if the walk is no self avoiding, return -1
        if (graph[row][col] != '.') return -1;

        // otherwise that spot is the next H or P in the prototein
        graph[row][col] = prototein[i];
    }

    // Actually scoring now.
    int score = 0;
    for (int i = 1; i < size - 1; i++) {
        for (int j = 1; j < size - 1; j++) {
            // if current spot is an H, and there is an H adjacent to it, score increments
            if (graph[i][j] == 'H' && graph[i+1][j] == 'H') score++;
            if (graph[i][j] == 'H' && graph[i][j+1] == 'H') score++;
            if (graph[i][j] == 'H' && graph[i-1][j] == 'H') score++;
            if (graph[i][j] == 'H' && graph[i][j-1] == 'H') score++;
        }
    }

    return score;
}

// initializing max and maxLabel variables for output
int maximum = -1;
int maxLabel = 0;


int main(int argc, char **argv) {
    char* proto = argv[1];
    // determine length of prototein (r)
    int r = strlen(proto);

    // Size is 2 minus r because length is 1 more than index, and for each walk you need to "walk" n-1 times
    int walk[r-2];

    unsigned long long start = rdtsc();
    // show all walks for a given prototein // pow(4,r-1) is 4^(r-1) because each turn has 4 possible outcomes
    for (int i = 0; i < (int) pow(4,r-1); i++) {
        // labelToWalk() takes the label, i, and turns it into base 4, creating every possible walk
        labelToWalk(i, walk, r);
        // Once label, i, and walk, base 4 i, are created, displayWalk() converts the base 4 walk into the N,E,S, and W walk
        //displayWalk(i, walk, r);
        // Time to score
        int s = score(proto, walk, r);
        //cout << "score: " << s << endl;
        if (s > maximum) {
            maximum = s;
            maxLabel = i;
        }
    }
    unsigned long long stop = rdtsc();
    cout << "Max: " << maximum << " " << maxLabel << endl;
    cout << "Runtime: " << stop - start << endl;
}
/*
This is a program the calculates the Maximum number of H-H contacts for an n-length prototein. This program is different than my
sequential and parallel versions because it has some optimizations in regards to the number of walks the program has to fully cycle through.
The two biggest optimizations being starting every walk by going north, and by only allowing walks to move either forward, left, or right.
Starting each walk by going north gives you the same end result because allowing starting in any direction yeilds 4 copies of the same walk,
just rotated 4 ways around the origin. This optimization reduces the total number of walks needing to be traversed and scored by a factor of 4.
It does not, however, reduce the number of self avoiding walks that are generated. That is where the second optimization comes into play. By
only allowing moves of forward, left, and right, it is impossible for a walk to immediately turn around and cross itself (i.e. north then south),
which would render it non-self-avoiding. This futher reduces the number of walks that need to be traversed and scored from 4^(n-1) to 3^(n-1).
That might see insignificant but lets say n=15,  4^(n-1) = 268,435,456 while 3^(n-1) = 4,782,969. Lastly, this program will display only the
maximum number of H-H contacts for a given n-length prototein as well as the run time. This is because, through testing, I have discovered
continuous output to slow down the program dramatically.

All code is my own, optimizations were taken from "How to Avoid Yourself" by Brian Hayes (1998), and implemented by me.

@author: Owen Sheed
*/
#include <iostream>
#include <math.h>
#include <string.h>
using namespace std;

#define FORWARD 0
#define LEFT 1
#define RIGHT 2

#define WEST 0
#define NORTH 1
#define EAST 2
#define SOUTH 3

char *prototein;
int protoLen;

unsigned long long rdtsc() {
   unsigned hi, lo;
   __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
   return ((unsigned long long) lo) | (((unsigned long long) hi) << 32);
}

void labelToWalk(int label,int *walk) {
    int count = 0;
    for (int i = protoLen - 2; i >= 0; i--){
        walk[count] = label / (int) pow(3, i);
        label = label % (int) pow(3, i);
        count++; 
    }
}

int score(char *prototein, int *walk) {
    // creating size measurement that is larger than 2 times our prototein. This is so we can iterate through the array without worrying about index out of bounds.
    int size = (2 * protoLen) + 1;

    // Creating the array
    char graph[size][size];

    // Initializing the array
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            graph[i][j] = '.';
        }
    }

    int row = protoLen;
    int col = protoLen;

    // putting the first part of the prototein at the center
    graph[row][col] = prototein[0];

    int fmove = NORTH;
    int lmove = WEST;
    int rmove = EAST;

    for (int i = 1; i < protoLen; i++) {

        // Navigating through the array to create the walk
        if (walk[i-1] == FORWARD){
            if (fmove == NORTH) row--; 
            if (fmove == SOUTH) row++;
            if (fmove == EAST ) col++;
            if (fmove == WEST ) col--;

            if (graph[row][col] != '.') return -1;
            graph[row][col] = prototein[i];
        }
        if (walk[i-1] == LEFT){
            if (lmove == NORTH) row--; 
            if (lmove == SOUTH) row++;
            if (lmove == EAST ) col++;
            if (lmove == WEST ) col--;

            if (graph[row][col] != '.') return -1;
            graph[row][col] = prototein[i];

            fmove = (fmove + 3) % 4;
            lmove = (lmove + 3) % 4;
            rmove = (rmove + 3) % 4;
        }
        if (walk[i-1] == RIGHT){
            if (rmove == NORTH) row--; 
            if (rmove == SOUTH) row++;
            if (rmove == EAST ) col++;
            if (rmove == WEST ) col--;

            if (graph[row][col] != '.') return -1;
            graph[row][col] = prototein[i];

            fmove = (fmove + 1) % 4;
            lmove = (lmove + 1) % 4;
            rmove = (rmove + 1) % 4;
        }
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

int maximum = -1;
int maxLabel = 0;

int main(int argc, char **argv){
    prototein = argv[1];
    protoLen = strlen(argv[1]);

    int numWalks = pow(3, protoLen-2);

    int walk[protoLen - 2];

    unsigned long long start = rdtsc();
    for (int i = 0; i < numWalks; i++){
        labelToWalk(i, walk);
        int s = score(prototein, walk);
        if (s > maximum) {
            maximum = s;
            maxLabel = i;
        }
    }
    unsigned long long stop = rdtsc();
    
    cout << maximum << " " << stop - start << endl;
}
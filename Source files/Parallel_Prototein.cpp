/*
This is a program the calculates the Maximum number of H-H contacts for an n-length prototein. It does this
by splitting up the walks across 20 threads. Each thread then has to create its portion of walks and score
them.

@author: Owen Sheed
*/
#include <iostream>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <cstdint>
using namespace std;

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3
#define NUMTHREADS 20

// Creating global variables
int numWalks;
int protoLen;
char *prototein;
int maximum = -1;
int maxLabel = 0;
pthread_mutex_t mutex;
pthread_mutex_t mutex2;

unsigned long long rdtsc() {
   unsigned hi, lo;
   __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
   return ((unsigned long long) lo) | (((unsigned long long) hi) << 32);
}

// turns base 10 label into base 4, creating every possible walk
void labelToWalk(int label,int *walk) {
    for (int p = protoLen-2; p >= 0; p--) {
        walk[p] = label / (int) pow(4, p);
        label = label % (int) pow(4, p);

    }
}

// Displays the walk in N,E,W, and S notation
void displayWalk(int label, int *walk) {
    cout << label << ": ";
    for (int i = protoLen-2; i >= 0; i--) {
        cout << walk[i];
    }
    cout << " ";
    for (int i = protoLen-2; i >= 0; i--) {
        if (walk[i] == NORTH) cout << "N";
        if (walk[i] == SOUTH) cout << "S";
        if (walk[i] == EAST)  cout << "E";
        if (walk[i] == WEST)  cout << "W";
    }
    cout << endl;
}


// Scores each walk
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

    for (int i = 1; i < protoLen; i++) {
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


void *parallel_func(void *threadid){
    uintptr_t tid = reinterpret_cast<uintptr_t>(threadid);
    int segmentSize = numWalks / NUMTHREADS;
    int startPos;
    int stopPos;
    int localMaximum = -1;
    int localMaxLabel = 0;

    // Divvying up the walks. Works with all combinations of prototein lengths and number of thread.
    if (tid < (NUMTHREADS - 1)) {
        startPos = segmentSize * tid;
        stopPos = (segmentSize * (tid + 1) - 1);
    } else {
        startPos = segmentSize * tid;
        stopPos = numWalks - 1;
    }
    
    // Creating the walk array. (protoLen - 2) because protoLen is 1 more than index, and we need (protoLen - 1) "moves".
    int walk[protoLen - 2];

    // Each thread is now going from its start to stop position
    for (int i = startPos; i <= stopPos; i++){
        labelToWalk(i, walk);
        int s = score(prototein, walk);

        // These mutex's ARE NOT required for this function to be thread safe. Keeping them in means cleaner output, but slower runtime as its called 4^(protoLen - 1) times.
        pthread_mutex_lock(&mutex);
        displayWalk(i, walk);
        cout << "score: " << s << endl;
        pthread_mutex_unlock(&mutex);

        if (s > localMaximum) {
            localMaximum = s;
            localMaxLabel = i;
        }
    }

    if (localMaximum > maximum) {
        // These mutex's ARE required for this function to be thread safe. Should not really impact performance because its only called 20 times.
        pthread_mutex_lock(&mutex2);
        maximum = localMaximum;
        maxLabel = localMaxLabel;
        pthread_mutex_unlock(&mutex2);
    }

    pthread_exit(NULL);
}


int main(int argc, char **argv){
    prototein = argv[1];

    long start = rdtsc();

    protoLen = strlen(argv[1]);
    numWalks = pow(4, protoLen-1);

    pthread_t threads[NUMTHREADS];
    pthread_mutex_init(&mutex, 0);
    pthread_mutex_init(&mutex2, 0);

    // Creating the threads
    for (long t = 0; t < NUMTHREADS; t++) {
        pthread_create(&threads[t], NULL, parallel_func, (void *)t);
    }

    // Waiting for the threads to finish
    for (long t = 0; t < NUMTHREADS; t++) {
        pthread_join(threads[t], NULL);
    }
    long stop = rdtsc();

    cout << "Max: " << maximum << " " << maxLabel << endl;
    cout << "Runtime: " << stop - start << endl;

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex2);
}
/*
This program exists to help me test runtime settings and the chronos library

@author: Owen Sheed
*/

#include <chrono>
#include <iostream>

std::chrono::time_point<std::chrono::high_resolution_clock> startTimer() {
    return std::chrono::high_resolution_clock::now();
}

void stopTimer(const std::chrono::time_point<std::chrono::high_resolution_clock>& start) {
    auto end = std::chrono::high_resolution_clock::now();
    long long int microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double seconds = microseconds / 1000000.0;
    std::cout << "Duration: " << microseconds << " microseconds\n";
    std::cout << "Duration: " << seconds << " seconds\n";
}



 
long long int fibonacci(int n)
{
    if (n < 2)
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}
 
int main(int argc, char **argv)
{
    int num;
    if (argc < 2){
        std::cout << "Type a number to compute" << std::endl;
        std::cin >> num;
    } else {
        num = std::stoi(argv[1]);
    }

    auto startTime = startTimer();
    long long int fibAns = fibonacci(num);
    std::cout << num << " digits of the finonacci sequence equals " << fibAns << std::endl;
    stopTimer(startTime);
}


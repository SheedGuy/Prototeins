/*
This program exists to help me test runtime settings and the chronos library

@author: Owen Sheed
*/

#include <chrono>
#include <iostream>

/* chrono function testing */
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
// Notes:
//  - Both functions have serious overhead that would mess throw off low end precision
//  - Calculating duration using time does not account for varying clock speeds


/* clock cycle function testing */
unsigned long long rdtsc() {
   unsigned hi, lo;
   __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
   return ((unsigned long long) lo) | (((unsigned long long) hi) << 32);
}
// Notes:
//  - Little overhead due to assembler code
//  - Only viable if CPU clock speed can be locked


 
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

    unsigned long long startTest = rdtsc();

    long long int fibAns = fibonacci(num);

    unsigned long long endTest = rdtsc();

    std::cout << "Clock Cycles: " << endTest - startTest << std::endl;
    std::cout << "Clock Cycles in seconds: " << (endTest - startTest) / 3992800000.0 << std::endl;

}


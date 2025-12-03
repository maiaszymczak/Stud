#include "PmergeMe.hpp"

double getTimeInMicroseconds(struct timeval start, struct timeval end)
{
    return (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);
}

void printUsage()
{
    std::cout << "Usage: ./PmergeMe <positive integers>" << std::endl;
}

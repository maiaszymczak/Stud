#include "PmergeMe.hpp"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printUsage();
        return 1;
    }

    std::vector<int> vec;
    std::deque<int> deq;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (!isPositiveInteger(arg))
        {
            std::cerr << RED << "Error" << RESET << std::endl;
            return 1;
        }
        int value = std::atoi(arg.c_str());
        vec.push_back(value);
        deq.push_back(value);
    }

    // Display "Before"
    std::cout << "Before: ";
    size_t displayCount = vec.size() > 5 ? 5 : vec.size();
    for (size_t i = 0; i < displayCount; ++i)
        std::cout << vec[i] << " ";
    if (vec.size() > 5)
        std::cout << "[...]";
    std::cout << std::endl;

    struct timeval startVec, endVec;
    gettimeofday(&startVec, NULL);
    mergeInsertSortVector(vec);
    gettimeofday(&endVec, NULL);
    double timeVec = getTimeInMicroseconds(startVec, endVec);

    struct timeval startDeq, endDeq;
    gettimeofday(&startDeq, NULL);
    mergeInsertSortDeque(deq);
    gettimeofday(&endDeq, NULL);
    double timeDeq = getTimeInMicroseconds(startDeq, endDeq);

    // Display "After"
    std::cout << "After: ";
    displayCount = vec.size() > 5 ? 5 : vec.size();
    for (size_t i = 0; i < displayCount; ++i)
        std::cout << vec[i] << " ";
    if (vec.size() > 5)
        std::cout << "[...]";
    std::cout << std::endl;

    std::cout << "Time to process a range of " << vec.size() 
              << " elements with std::vector : " 
              << std::fixed << std::setprecision(5) << timeVec << " us" << std::endl;
    std::cout << "Time to process a range of " << deq.size() 
              << " elements with std::deque : " 
              << std::fixed << std::setprecision(5) << timeDeq << " us" << std::endl;

    return 0;
}

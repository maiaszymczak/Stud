#include "PmergeMe.hpp"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printUsage();
        return 1;
    }

    try {
        std::vector<int> vec;
        std::deque<int> deq;

        fill_stack(argc, argv, vec);
        fill_stack(argc, argv, deq);

        
        std::cout << "Before: ";
        size_t displayCount = vec.size() > 5 ? 5 : vec.size();
        for (size_t i = 0; i < displayCount; ++i)
            std::cout << vec[i] << " ";
        if (vec.size() > 5)
            std::cout << "[...]";
        std::cout << std::endl;

        
        std::vector<int> finalVec;
        struct timeval startVec, endVec;
        gettimeofday(&startVec, NULL);
        PmergeMe<std::vector<int>, std::vector<std::pair<int, int> > >(vec, finalVec);
        gettimeofday(&endVec, NULL);
        double timeVec = getTimeInMicroseconds(startVec, endVec);

        
        std::deque<int> finalDeq;
        struct timeval startDeq, endDeq;
        gettimeofday(&startDeq, NULL);
        PmergeMe<std::deque<int>, std::vector<std::pair<int, int> > >(deq, finalDeq);
        gettimeofday(&endDeq, NULL);
        double timeDeq = getTimeInMicroseconds(startDeq, endDeq);

       
        std::cout << "After: ";
        displayCount = finalVec.size() > 5 ? 5 : finalVec.size();
        for (size_t i = 0; i < displayCount; ++i)
            std::cout << finalVec[i] << " ";
        if (finalVec.size() > 5)
            std::cout << "[...]";
        std::cout << std::endl;

        std::cout << "Time to process a range of " << finalVec.size() 
                  << " elements with std::vector : " 
                  << std::fixed << std::setprecision(5) << timeVec << " us" << std::endl;
        std::cout << "Time to process a range of " << finalDeq.size() 
                  << " elements with std::deque : " 
                  << std::fixed << std::setprecision(5) << timeDeq << " us" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << RED << "Error" << RESET << std::endl;
        return 1;
    }

    return 0;
}

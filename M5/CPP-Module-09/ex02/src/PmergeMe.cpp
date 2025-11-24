#include "PmergeMe.hpp"

double getTimeInMicroseconds(struct timeval start, struct timeval end)
{
    return (end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec);
}

void printUsage()
{
    std::cout << "Usage: ./PmergeMe <positive integers>" << std::endl;
}

bool isPositiveInteger(const std::string& str)
{
    for (size_t i = 0; i < str.length(); ++i)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return !str.empty();
}

// Generate Jacobsthal sequence for optimal insertion order
std::vector<size_t> generateJacobsthalSequence(size_t n)
{
    std::vector<size_t> jacobsthal;
    if (n == 0)
        return jacobsthal;
    
    // Generate Jacobsthal numbers: J(0)=0, J(1)=1, J(n)=J(n-1)+2*J(n-2)
    std::vector<size_t> jacob;
    jacob.push_back(0);
    jacob.push_back(1);
    
    while (jacob.back() < n)
    {
        size_t next = jacob[jacob.size() - 1] + 2 * jacob[jacob.size() - 2];
        jacob.push_back(next);
    }
    
    // Build the insertion sequence based on Jacobsthal numbers
    std::vector<bool> used(n, false);
    
    for (size_t i = 3; i < jacob.size() && jacob[i] <= n; ++i)
    {
        size_t current = jacob[i];
        if (current <= n && current > 0 && !used[current - 1])
        {
            jacobsthal.push_back(current - 1);
            used[current - 1] = true;
        }
        
        // Fill in the gaps between Jacobsthal numbers in reverse
        size_t prev = jacob[i - 1];
        for (size_t j = current - 1; j > prev && j > 0; --j)
        {
            if (j <= n && !used[j - 1])
            {
                jacobsthal.push_back(j - 1);
                used[j - 1] = true;
            }
        }
    }
    
    // Add any remaining elements
    for (size_t i = 0; i < n; ++i)
    {
        if (!used[i])
            jacobsthal.push_back(i);
    }
    
    return jacobsthal;
}

// Binary search insertion for sorted vector
void binaryInsertVector(std::vector<int>& sorted, int value, size_t end)
{
    if (end == 0)
    {
        sorted.insert(sorted.begin(), value);
        return;
    }
    
    size_t left = 0;
    size_t right = end;
    
    while (left < right)
    {
        size_t mid = left + (right - left) / 2;
        if (sorted[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    
    sorted.insert(sorted.begin() + left, value);
}

// Ford-Johnson algorithm for vector
void fordJohnsonVector(std::vector<int>& vec)
{
    size_t n = vec.size();
    
    if (n <= 1)
        return;
    
    // Handle odd number of elements
    int straggler = -1;
    bool hasStraggler = (n % 2 != 0);
    if (hasStraggler)
    {
        straggler = vec.back();
        vec.pop_back();
        n--;
    }
    
    // Step 1: Create pairs and sort each pair
    std::vector<std::pair<int, int> > pairs;
    for (size_t i = 0; i < n; i += 2)
    {
        if (vec[i] < vec[i + 1])
            pairs.push_back(std::make_pair(vec[i], vec[i + 1]));
        else
            pairs.push_back(std::make_pair(vec[i + 1], vec[i]));
    }
    
    // Step 2: Sort pairs by their larger element (simple insertion sort)
    for (size_t i = 1; i < pairs.size(); ++i)
    {
        std::pair<int, int> key = pairs[i];
        size_t j = i;
        while (j > 0 && pairs[j - 1].second > key.second)
        {
            pairs[j] = pairs[j - 1];
            --j;
        }
        pairs[j] = key;
    }
    
    // Step 3: Build main chain with larger elements
    std::vector<int> mainChain;
    mainChain.push_back(pairs[0].first); // First smaller element goes in
    for (size_t i = 0; i < pairs.size(); ++i)
        mainChain.push_back(pairs[i].second);
    
    // Step 4: Insert smaller elements using Jacobsthal sequence
    std::vector<int> pend;
    for (size_t i = 1; i < pairs.size(); ++i)
        pend.push_back(pairs[i].first);
    
    std::vector<size_t> insertOrder = generateJacobsthalSequence(pend.size());
    
    for (size_t i = 0; i < insertOrder.size(); ++i)
    {
        size_t idx = insertOrder[i];
        if (idx < pend.size())
        {
            int value = pend[idx];
            // Binary search up to where this element's pair is
            size_t searchEnd = idx + i + 2; // Adjusted search range
            if (searchEnd > mainChain.size())
                searchEnd = mainChain.size();
            binaryInsertVector(mainChain, value, searchEnd);
        }
    }
    
    // Step 5: Insert straggler if exists
    if (hasStraggler)
        binaryInsertVector(mainChain, straggler, mainChain.size());
    
    vec = mainChain;
}

void mergeInsertSortVector(std::vector<int>& vec)
{
    fordJohnsonVector(vec);
}

// Binary search insertion for sorted deque
void binaryInsertDeque(std::deque<int>& sorted, int value, size_t end)
{
    if (end == 0)
    {
        sorted.push_front(value);
        return;
    }
    
    size_t left = 0;
    size_t right = end;
    
    while (left < right)
    {
        size_t mid = left + (right - left) / 2;
        if (sorted[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    
    sorted.insert(sorted.begin() + left, value);
}

// Ford-Johnson algorithm for deque
void fordJohnsonDeque(std::deque<int>& deq)
{
    size_t n = deq.size();
    
    if (n <= 1)
        return;
    
    // Handle odd number of elements
    int straggler = -1;
    bool hasStraggler = (n % 2 != 0);
    if (hasStraggler)
    {
        straggler = deq.back();
        deq.pop_back();
        n--;
    }
    
    // Step 1: Create pairs and sort each pair
    std::vector<std::pair<int, int> > pairs;
    for (size_t i = 0; i < n; i += 2)
    {
        if (deq[i] < deq[i + 1])
            pairs.push_back(std::make_pair(deq[i], deq[i + 1]));
        else
            pairs.push_back(std::make_pair(deq[i + 1], deq[i]));
    }
    
    // Step 2: Sort pairs by their larger element (simple insertion sort)
    for (size_t i = 1; i < pairs.size(); ++i)
    {
        std::pair<int, int> key = pairs[i];
        size_t j = i;
        while (j > 0 && pairs[j - 1].second > key.second)
        {
            pairs[j] = pairs[j - 1];
            --j;
        }
        pairs[j] = key;
    }
    
    // Step 3: Build main chain with larger elements
    std::deque<int> mainChain;
    mainChain.push_back(pairs[0].first); // First smaller element goes in
    for (size_t i = 0; i < pairs.size(); ++i)
        mainChain.push_back(pairs[i].second);
    
    // Step 4: Insert smaller elements using Jacobsthal sequence
    std::vector<int> pend;
    for (size_t i = 1; i < pairs.size(); ++i)
        pend.push_back(pairs[i].first);
    
    std::vector<size_t> insertOrder = generateJacobsthalSequence(pend.size());
    
    for (size_t i = 0; i < insertOrder.size(); ++i)
    {
        size_t idx = insertOrder[i];
        if (idx < pend.size())
        {
            int value = pend[idx];
            // Binary search up to where this element's pair is
            size_t searchEnd = idx + i + 2; // Adjusted search range
            if (searchEnd > mainChain.size())
                searchEnd = mainChain.size();
            binaryInsertDeque(mainChain, value, searchEnd);
        }
    }
    
    // Step 5: Insert straggler if exists
    if (hasStraggler)
        binaryInsertDeque(mainChain, straggler, mainChain.size());
    
    deq = mainChain;
}

void mergeInsertSortDeque(std::deque<int>& deq)
{
    fordJohnsonDeque(deq);
}

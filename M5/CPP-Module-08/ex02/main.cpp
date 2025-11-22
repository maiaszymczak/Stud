#include "MutantStack.hpp"
#include <list>

int main()
{
    std::cout << BRIGHT_YELLOW "=== Testing MutantStack ===" RESET << std::endl;
    
    MutantStack<int> mstack;
    
    mstack.push(5);
    mstack.push(17);

    std::cout << BRIGHT_CYAN "Top element: " << mstack.top() << RESET << std::endl;

    mstack.pop();

    std::cout << BRIGHT_CYAN "Size after pop: " << mstack.size() << RESET << std::endl;

    mstack.push(3);
    mstack.push(5);
    mstack.push(737);
    mstack.push(0);

    std::cout << BRIGHT_CYAN "\nIterating through MutantStack:" << RESET << std::endl;
    MutantStack<int>::iterator it = mstack.begin();
    MutantStack<int>::iterator ite = mstack.end();
    ++it;
    --it;
    while (it != ite)
    {
        std::cout << PURPLE << *it << RESET << std::endl;
        ++it;
    }

    std::cout << BRIGHT_CYAN "\nReverse iterating through MutantStack:" << RESET << std::endl;
    for (MutantStack<int>::reverse_iterator rit = mstack.rbegin(); rit != mstack.rend(); ++rit)
    {
        std::cout << PURPLE << *rit << RESET << std::endl;
    }

    std::cout << "\n" BRIGHT_YELLOW "=== Testing std::list (same operations) ===" RESET << std::endl;
    
    std::list<int> lst;
    
    lst.push_back(5);
    lst.push_back(17);

    std::cout << BRIGHT_CYAN "Back element: " << lst.back() << RESET << std::endl;

    lst.pop_back();

    std::cout << BRIGHT_CYAN "Size after pop: " << lst.size() << RESET << std::endl;

    lst.push_back(3);
    lst.push_back(5);
    lst.push_back(737);
    lst.push_back(0);

    std::cout << BRIGHT_CYAN "\nIterating through std::list:" << RESET << std::endl;
    std::list<int>::iterator lit = lst.begin();
    std::list<int>::iterator lite = lst.end();
    ++lit;
    --lit;
    while (lit != lite)
    {
        std::cout << PURPLE << *lit << RESET << std::endl;
        ++lit;
    }

    std::cout << BRIGHT_CYAN "\nReverse iterating through std::list:" << RESET << std::endl;
    for (std::list<int>::reverse_iterator rlit = lst.rbegin(); rlit != lst.rend(); ++rlit)
    {
        std::cout << PURPLE << *rlit << RESET << std::endl;
    }

    std::cout << "\n" BRIGHT_GREEN "same output!" RESET << std::endl;

    std::cout << "\n" BRIGHT_YELLOW "=== Testing copy constructor ===" RESET << std::endl;
    MutantStack<int> mstack_copy(mstack);
    std::cout << BRIGHT_CYAN "Copy size: " << mstack_copy.size() << RESET << std::endl;

    std::cout << "\n" BRIGHT_YELLOW "=== Testing assignment operator ===" RESET << std::endl;
    MutantStack<int> mstack_assigned;
    mstack_assigned = mstack;
    std::cout << BRIGHT_CYAN "Assigned size: " << mstack_assigned.size() << RESET << std::endl;

    return 0;
}

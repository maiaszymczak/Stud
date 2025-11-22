#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include "Colors.hpp"
#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>

Base* generate(void) {
    int nb = std::rand();
    
    if (nb % 6 == 0) {
        return new A();
    } else if (nb % 4 == 0) {
        return new B();
    } else {
        return new C();
    }
}

std::string identify(Base* p) {
    if (dynamic_cast<A*>(p))
        return "A";
    else if (dynamic_cast<B*>(p))
        return "B";
    else if (dynamic_cast<C*>(p))
        return "C";
    else
        return "Unknown";
}

std::string identify(Base& p) {
    try {
        (void)dynamic_cast<A&>(p);
        return "A";
    } catch (std::exception& e) {}
    
    try {
        (void)dynamic_cast<B&>(p);
        return "B";
    } catch (std::exception& e) {}
    
    try {
        (void)dynamic_cast<C&>(p);
        return "C";
    } catch (std::exception& e) {}
    
    return "Unknown";
}

int main() {
    std::srand(std::time(NULL));
    
    int numTests;
    std::cout << BRIGHT_MAGENTA << "How many random tests do you want to run? " << RESET;
    std::cin >> numTests;
    
    if (numTests <= 0 || std::cin.fail()) {
        std::cout << RED << "Invalid number, using default: 3" << RESET << std::endl;
        numTests = 3;
        std::cin.clear();
    }
    
    int successCount = 0;
    
    for (int i = 1; i <= numTests; i++) {
        std::cout << BOLD << "\n=== Random Test " << i << " ===" << RESET << std::endl;
        Base* base = generate();
        
        std::string actualType;
        if (dynamic_cast<A*>(base))
            actualType = "A";
        else if (dynamic_cast<B*>(base))
            actualType = "B";
        else if (dynamic_cast<C*>(base))
            actualType = "C";
        
        std::cout << BRIGHT_MAGENTA << "→ Generated class " << actualType << RESET << std::endl;
        
        std::string typeByPtr = identify(base);
        std::string typeByRef = identify(*base);
        
        std::cout << PURPLE << "Identified by pointer: " << RESET;
        if (typeByPtr == actualType)
            std::cout << GREEN << typeByPtr << " ✓" << RESET << std::endl;
        else
            std::cout << RED << typeByPtr << " ✗" << RESET << std::endl;
        
        std::cout << ORANGE << "Identified by reference: " << RESET;
        if (typeByRef == actualType)
            std::cout << GREEN << typeByRef << " ✓" << RESET << std::endl;
        else
            std::cout << RED << typeByRef << " ✗" << RESET << std::endl;
        
        if (typeByPtr == actualType && typeByRef == actualType)
            successCount++;
        
        delete base;
    }
    
    std::cout << BOLD << "\n=== Results: " << successCount << "/" << numTests << " tests passed ===" << RESET << std::endl;
    
    std::cout << BOLD << "\n=== Manual verification (A, B, C) ===" << RESET << std::endl;
    Base* a = new A();
    Base* b = new B();
    Base* c = new C();
    
    std::cout << BOLD << "\nTesting A:" << RESET << std::endl;
    std::cout << PURPLE << "By pointer: " << RESET << GREEN << identify(a) << RESET << std::endl;
    std::cout << ORANGE << "By reference: " << RESET << GREEN << identify(*a) << RESET << std::endl;
    
    std::cout << BOLD << "\nTesting B:" << RESET << std::endl;
    std::cout << PURPLE << "By pointer: " << RESET << GREEN << identify(b) << RESET << std::endl;
    std::cout << ORANGE << "By reference: " << RESET << GREEN << identify(*b) << RESET << std::endl;
    
    std::cout << BOLD << "\nTesting C:" << RESET << std::endl;
    std::cout << PURPLE << "By pointer: " << RESET << GREEN << identify(c) << RESET << std::endl;
    std::cout << ORANGE << "By reference: " << RESET << GREEN << identify(*c) << RESET << std::endl;
    
    delete a;
    delete b;
    delete c;

    return 0;
}
#include "iter.hpp"
#include "Colors.hpp"
#include <iostream>
#include <string>

template <typename A>
void print(A &value) {
	std::cout << PINK << "[" << NPURPLE << value << PINK << "]" << RESET << " ";
}

template <typename A>
void incrementValue(A &value) {
	++value;
}

template <typename A>
void multiplyByTwo(A &value) {
	value *= 2;
}

template <typename A>
void upper(A &value) {
	for (int i = 0; value[i]; i++)
		value[i] = std::toupper(value[i]);
}

int main() {
	
	// Test 1: Array of integers
	int numbers[] = {5, 10, 15, 20, 25, 30};
	
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	std::cout << BOLD << CYAN << "Test 1: Integer Array Operations" << RESET << std::endl;
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	
	std::cout << YELLOW << "  Original values: " << RESET;
	iter(numbers, 6, print<int>);
	std::cout << std::endl;
	
	iter(numbers, 6, multiplyByTwo<int>);
	std::cout << YELLOW << "  After x2:        " << RESET;
	iter(numbers, 6, print<int>);
	std::cout << "\n" << std::endl;

	// Test 2: Array of strings
	std::string words[] = {"hello", "beautiful", "world", "from", "cpp", "module"};
	
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	std::cout << BOLD << MAGENTA << "Test 2: String Array Transformation" << RESET << std::endl;
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	
	std::cout << YELLOW << "  Original strings: " << RESET;
	iter(words, 6, print<std::string>);
	std::cout << std::endl;
	
	iter(words, 6, upper<std::string>);
	std::cout << YELLOW << "  After uppercase:  " << RESET;
	iter(words, 6, print<std::string>);
	std::cout << "\n" << std::endl;

	// Test 3: Array of floats
	float scores[] = {7.5f, 8.2f, 9.1f, 6.8f, 8.9f};
	
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	std::cout << BOLD << ORANGE << "Test 3: Float Array (Scores)" << RESET << std::endl;
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	
	std::cout << YELLOW << "  Values:   " << RESET;
	iter(scores, 5, print<float>);
	std::cout << std::endl;
	
	iter(scores, 5, incrementValue<float>);
	std::cout << YELLOW << "  +1:       " << RESET;
	iter(scores, 5, print<float>);
	std::cout << "\n" << std::endl;

	std::cout << BOLD << BRIGHT_MAGENTA << "I don't have any more idea for testing sorry !" << RESET << "\n" << std::endl;

	return 0;
}


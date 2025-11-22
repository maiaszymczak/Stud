#include "Array.hpp"
#include "Colors.hpp"
#include <iostream>
#include <string>

template <typename T>
void printArray(const Array<T>& arr, const std::string& name) {
	std::cout << CYAN << name << RESET << " (size=" << arr.size() << "): ";
	if (arr.size() == 0) {
		std::cout << YELLOW << "[empty]" << RESET;
	} else {
		std::cout << GREEN << "[" << RESET;
		for (unsigned int i = 0; i < arr.size(); i++) {
			std::cout << NGREEN << arr[i] << RESET;
			if (i < arr.size() - 1)
				std::cout << ", ";
		}
		std::cout << GREEN << "]" << RESET;
	}
	std::cout << std::endl;
}

int main()
{

	// Test 1: Default constructor (empty array)
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	std::cout << BOLD << CYAN << "Test 1: Default Constructor (Empty Array)" << RESET << std::endl;
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	
	Array<int> emptyArray;
	printArray(emptyArray, "emptyArray");
	std::cout << std::endl;

	// Test 2: Constructor with size parameter
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	std::cout << BOLD << MAGENTA << "Test 2: Constructor with Size Parameter" << RESET << std::endl;
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	
	Array<int> intArray(5);
	std::cout << YELLOW << "  Created array of 5 ints (default initialized):" << RESET << std::endl;
	printArray(intArray, "  intArray");
	
	std::cout << YELLOW << "\n  Modifying values:" << RESET << std::endl;
	for (unsigned int i = 0; i < intArray.size(); i++)
		intArray[i] = (i + 1) * 10;
	printArray(intArray, "  intArray");
	std::cout << std::endl;

	// Test 3: Copy constructor (deep copy)
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	std::cout << BOLD << ORANGE << "Test 3: Copy Constructor (Deep Copy)" << RESET << std::endl;
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	
	Array<int> copiedArray(intArray);
	std::cout << YELLOW << "  Original array:" << RESET << std::endl;
	printArray(intArray, "  intArray");
	std::cout << YELLOW << "  Copied array:" << RESET << std::endl;
	printArray(copiedArray, "  copiedArray");
	
	std::cout << YELLOW << "\n  Modifying original array (multiply by 2):" << RESET << std::endl;
	for (unsigned int i = 0; i < intArray.size(); i++)
		intArray[i] *= 2;
	
	printArray(intArray, "  intArray");
	std::cout << YELLOW << "  Copied array should remain unchanged:" << RESET << std::endl;
	printArray(copiedArray, "  copiedArray");
	std::cout << std::endl;

	// Test 4: Assignment operator
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	std::cout << BOLD << PINK << "Test 4: Assignment Operator" << RESET << std::endl;
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	
	Array<int> assignedArray;
	assignedArray = copiedArray;
	std::cout << YELLOW << "  Assigned array:" << RESET << std::endl;
	printArray(assignedArray, "  assignedArray");
	
	std::cout << YELLOW << "\n  Modifying assigned array (add 100):" << RESET << std::endl;
	for (unsigned int i = 0; i < assignedArray.size(); i++)
		assignedArray[i] += 100;
	
	printArray(assignedArray, "  assignedArray");
	std::cout << YELLOW << "  Original should remain unchanged:" << RESET << std::endl;
	printArray(copiedArray, "  copiedArray");
	std::cout << std::endl;

	// Test 5: String array
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	std::cout << BOLD << BRIGHT_YELLOW << "Test 5: Array of Strings" << RESET << std::endl;
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	
	Array<std::string> strArray(4);
	strArray[0] = "Hello";
	strArray[1] = "from";
	strArray[2] = "Array";
	strArray[3] = "class!";
	
	printArray(strArray, "  strArray");
	std::cout << std::endl;

	// Test 6: Exception handling (out of bounds)
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	std::cout << BOLD << RED << "Test 6: Exception Handling (Out of Bounds)" << RESET << std::endl;
	std::cout << NPURPLE << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << RESET << std::endl;
	
	try {
		std::cout << YELLOW << "  Trying to access index 10 in array of size 5..." << RESET << std::endl;
		int value = intArray[10];
		std::cout << RED << "  ✗ No exception thrown! Value: " << value << RESET << std::endl;
	} catch (std::exception& e) {
		std::cout << NGREEN << "  ✓ Exception caught: " << e.what() << RESET << std::endl;
	}
	
	try {
		std::cout << YELLOW << "\n  Trying to access index 0 in empty array..." << RESET << std::endl;
		int value = emptyArray[0];
		std::cout << RED << "  ✗ No exception thrown! Value: " << value << RESET << std::endl;
	} catch (std::exception& e) {
		std::cout << NGREEN << "  ✓ Exception caught: " << e.what() << RESET << std::endl;
	}
	std::cout << std::endl;

	std::cout << BOLD << NGREEN << "✓ All tests completed successfully!" << RESET << "\n" << std::endl;

	return 0;
}

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sys/time.h>
#include "Colors.hpp"

void printUsage();
bool isPositiveInteger(const std::string& str);
void mergeInsertSortVector(std::vector<int>& vec);
void mergeInsertSortDeque(std::deque<int>& deq);
double getTimeInMicroseconds(struct timeval start, struct timeval end);

// Ford-Johnson helper functions for vector
std::vector<size_t> generateJacobsthalSequence(size_t n);
void fordJohnsonVector(std::vector<int>& vec);

// Ford-Johnson helper functions for deque
void fordJohnsonDeque(std::deque<int>& deq);

#endif
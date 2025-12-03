#include "Span.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>

Span::Span() : _tab() {
  _size = 0;
  std::cout << GREEN << "Span Default Constructor called" << RESET << std::endl;
}

Span::Span(unsigned int N) : _size(N) {
  if (N == 0)
    throw std::invalid_argument("invalid argument");
  _tab.reserve(N);
  std::cout << GREEN << "Span Default Constructor called" << RESET << std::endl;
}

Span::Span(const Span &src) {
  std::cout << BLUE << "Span Copy Constructor called" << RESET << std::endl;
  *this = src;
}

Span::~Span() {
  std::cout << RED << "Span Destructor called " << RESET << std::endl;
}

Span &Span::operator=(const Span &src) {
  std::cout << PURPLE << "Span Copy Assignment Operator called" << RESET
            << std::endl;
  if (this != &src) {
    this->_size = src._size;
    this->_tab = src._tab;
  }
  return *this;
}

int Span::shortestSpan() {
  if (_tab.size() < 2)
    throw std::invalid_argument("empty array");
  
  std::vector<int> sorted(_tab);
  std::sort(sorted.begin(), sorted.end());
  
  std::vector<int> differences(sorted.size() - 1);
  std::adjacent_difference(sorted.begin(), sorted.end(), differences.begin());
  
  int shortest = *std::min_element(differences.begin() + 1, differences.end());
  
  std::cout << NGREEN << "the shortest span is " << shortest << RESET
            << std::endl;
  return shortest;
}

int Span::longestSpan() {
  if (_tab.size() < 2)
    throw std::invalid_argument("empty array");
  
  int min = *std::min_element(_tab.begin(), _tab.end());
  int max = *std::max_element(_tab.begin(), _tab.end());
  
  std::cout << NGREEN << "the longest span is " << max - min << RESET
            << std::endl;
  return max - min;
}

void Span::addNumber(int nb) {
  if (_tab.size() < _size) {
    _tab.push_back(nb);
    std::cout << NGREEN << nb << " added to list" << RESET << std::endl;
  } else {
    throw std::invalid_argument("array is full");
  }
}

void Span::addManyNumbers(std::vector<int> &vec) {
  if (_size - _tab.size() < vec.size())
    throw std::invalid_argument("not enough space to add Numbers");
  _tab.insert(_tab.end(), vec.begin(), vec.end());
  std::cout << NGREEN << "Array added:" << RESET << std::endl;
  for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
    std::cout << GREEN << "[" << *it << "]" << RESET << std::endl;
}

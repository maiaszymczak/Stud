#ifndef SPAN_HPP
#define SPAN_HPP

#include <vector>
#include "Colors.hpp"
#include <stdexcept>
#include <iostream>

class Span {
public:

  Span(unsigned int N);
  Span(const Span &src);
  ~Span();
  Span &operator=(const Span &src);

  int shortestSpan();
  int longestSpan();
  void addNumber(int nb);
  void addManyNumbers(std::vector<int> &vec);

private:
  Span();
  std::vector<int> _tab;
  unsigned int _size;
};

#endif
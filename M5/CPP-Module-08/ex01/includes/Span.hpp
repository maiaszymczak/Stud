#ifndef SPAN_HPP
# define SPAN_HPP

# include <iostream>
# include <vector>
# include <algorithm>
# include <stdexcept>
# include <limits>
# include "Colors.hpp"

class Span
{
public:
    Span(unsigned int n);
    Span(const Span &other);
    Span &operator=(const Span &other);
    ~Span();

    void addNumber(int number);
    void addNumbers(std::vector<int>::iterator begin, std::vector<int>::iterator end);
    unsigned int shortestSpan() const;
    unsigned int longestSpan() const;
private:
    unsigned int _maxSize;
    std::vector<int> _numbers;
};

#endif
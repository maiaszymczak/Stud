#include "Span.hpp"

Span::Span(unsigned int n) : _maxSize(n)
{
    std::cout << NGREEN << "Span constructor called with max size: " << n << RESET << std::endl;
}

Span::Span(const Span &other) : _maxSize(other._maxSize), _numbers(other._numbers)
{
    std::cout << BLUE << "Span copy constructor called" << RESET << std::endl;
}

Span &Span::operator=(const Span &other)
{
    if (this != &other)
    {
        std::cout << YELLOW << "Span assignment operator called" << RESET << std::endl;
        _maxSize = other._maxSize;
        _numbers = other._numbers;
    }
    return *this;
}

Span::~Span()
{
    std::cout << RED << "Span destructor called" << RESET << std::endl;
}

void Span::addNumber(int number)
{
    if (_numbers.size() >= _maxSize)
        throw std::overflow_error("Cannot add more numbers, capacity reached");
    _numbers.push_back(number);
}

void Span::addNumbers(std::vector<int>::iterator begin, std::vector<int>::iterator end)
{
    if (std::distance(begin, end) + _numbers.size() > static_cast<std::ptrdiff_t>(_maxSize))
        throw std::overflow_error("Cannot add more numbers, capacity reached");
    _numbers.insert(_numbers.end(), begin, end);
}
unsigned int Span::shortestSpan() const
{
    if (_numbers.size() < 2)
        throw std::logic_error("Not enough numbers to find a span");

    std::vector<int> sortedNumbers = _numbers;
    std::sort(sortedNumbers.begin(), sortedNumbers.end());

    unsigned int minSpan = std::numeric_limits<unsigned int>::max();
    for (size_t i = 1; i < sortedNumbers.size(); ++i)
    {
        unsigned int span = static_cast<unsigned int>(sortedNumbers[i] - sortedNumbers[i - 1]);
        if (span < minSpan)
            minSpan = span;
    }
    return minSpan;
}

unsigned int Span::longestSpan() const
{
    if (_numbers.size() < 2)
        throw std::logic_error("Not enough numbers to find a span");

    int minNumber = *std::min_element(_numbers.begin(), _numbers.end());
    int maxNumber = *std::max_element(_numbers.begin(), _numbers.end());

    return static_cast<unsigned int>(maxNumber - minNumber);
}

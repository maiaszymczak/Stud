#ifndef SCALARCONVERTER_HPP
#define SCALARCONVERTER_HPP

#include <string>
#include <iostream>
#include <string>
#include <cstdlib>
#include <limits>
#include <cmath>
#include <iomanip>
#include "Colors.hpp"

class ScalarConverter 
{
    private:
	    ScalarConverter();
	    ScalarConverter(const ScalarConverter& other);
	    ScalarConverter& operator=(const ScalarConverter& other);
	    ~ScalarConverter();

    public:
        static void convert(const std::string& literal);
};

#endif

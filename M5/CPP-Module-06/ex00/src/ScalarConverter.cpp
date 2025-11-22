#include "ScalarConverter.hpp"

ScalarConverter::ScalarConverter() {
	std::cout << GREEN << "ScalarConverter Default Constructor called" << RESET << std::endl;
}

ScalarConverter::ScalarConverter(const ScalarConverter &src) {
	std::cout << BLUE << "ScalarConverter Copy Constructor called" << RESET << std::endl;
	*this = src;
}

ScalarConverter::~ScalarConverter() {
	std::cout << RED << "ScalarConverter Destructor called " << RESET << std::endl;
}

ScalarConverter &ScalarConverter::operator=(const ScalarConverter &src) {
	std::cout << PURPLE << "ScalarConverter Copy Assignment Operator called" << RESET << std::endl;
	if (this != &src) {
	}
	return *this;
}

static bool only_one_char(std::string arg) {
	if (arg[0] >= 32 && arg[0] <= 126 && !arg[1])
		return true;
	return false;
}

static bool only_digit_string(std::string arg) {
	int i = 0;
	for (; arg[i]; i++) {
		if (!std::isdigit(arg[i]))
			return false;
	}
	return true;
}

static void printchar(std::string arg) {
	long double d = strtold(arg.c_str(), NULL);
	if (d < std::numeric_limits<char>::min() || d > std::numeric_limits<char>::max() ||
		std::isnan(d))
		std::cout << ORANGE << "char = impossible" << RESET << std::endl;
	else if ((only_digit_string(arg) && (!std::isprint(d))) || arg[0] == '-')
		std::cout << ORANGE << "char = non displayable" << RESET << std::endl;
	else if (only_one_char(arg))
		std::cout << ORANGE << "char = " << static_cast<char>(arg[0]) << RESET << std::endl;
	else
		std::cout << ORANGE << "char = " << static_cast<char>(d) << RESET << std::endl;
}

static void printint(std::string arg) {
	long double d = strtold(arg.c_str(), NULL);
	if (d < std::numeric_limits<int>::min() || d > std::numeric_limits<int>::max() ||
		std::isnan(d) || (d == 0 && arg[0] != '0'))
		std::cout << BLUE << "int = impossible" << RESET << std::endl;
	else
		std::cout << ORANGE << "int = " << static_cast<int>(d) << RESET << std::endl;
}

static void printfloat(std::string arg) {
	long double d = strtold(arg.c_str(), NULL);
	if (std::isnan(d))
		std::cout << PURPLE << "float = nanf" << RESET << std::endl;
	else if (d == 0 && arg[0] != '0')
		std::cout << PURPLE << "float = impossible" << RESET << std::endl;
	else {
		float f = static_cast<float>(d);
		std::cout << PURPLE << "float = " << f;
		if (f == static_cast<int>(f))
			std::cout << ".0";
		std::cout << "f" << RESET << std::endl;
	}
}

static void printdouble(std::string arg) {
	long double d = strtold(arg.c_str(), NULL);
	if (std::isnan(d))
		std::cout << PURPLE << "double = nan" << RESET << std::endl;
	else if (d == 0 && arg[0] != '0')
		std::cout << PURPLE << "double = impossible" << RESET << std::endl;
	else {
		double dbl = static_cast<double>(d);
		std::cout << PURPLE << "double = " << dbl;
		if (dbl == static_cast<int>(dbl))
			std::cout << ".0";
		std::cout << RESET << std::endl;
	}
}

void ScalarConverter::convert(const std::string& literal) {
	printchar(literal);
	printint(literal);
	printfloat(literal);
	printdouble(literal);
}

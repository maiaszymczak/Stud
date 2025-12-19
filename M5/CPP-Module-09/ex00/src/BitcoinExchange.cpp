#include "BitcoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>


Date::Date() : year(0), month(0), day(0) {}

Date::~Date() {}
Date::Date(const std::string& dateStr)
{
    sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);
}

Date::Date(const Date& other) : year(other.year), month(other.month), day(other.day) {}

Date& Date::operator=(const Date& other)
{
    if (this != &other)
    {
        year = other.year;
        month = other.month;
        day = other.day;
    }
    return *this;
}

bool Date::operator<(const Date& other) const
{
    if (year != other.year)
        return year < other.year;
    if (month != other.month)
        return month < other.month;
    return day < other.day;
}

bool Date::operator==(const Date& other) const
{
    return year == other.year && month == other.month && day == other.day;
}

int Date::getYear() const { return year; }
int Date::getMonth() const { return month; }
int Date::getDay() const { return day; }
void Date::setYear(int y) { year = y; }
void Date::setMonth(int m) { month = m; }
void Date::setDay(int d) { day = d; }

std::ostream& operator<<(std::ostream& os, const Date& date)
{
    os << date.getYear() << "-";
    if (date.getMonth() < 10) os << "0";
    os << date.getMonth() << "-";
    if (date.getDay() < 10) os << "0";
    os << date.getDay();
    return os;
}

std::map<Date, double> loadDataFromCSV(const std::string& filename)
{
    std::map<Date, double> dataMap;
    std::ifstream file(filename.c_str());
    if (!file.is_open())
    {
        std::cerr << RED << "Error: could not open file." << RESET << std::endl;
        return dataMap;
    }
    std::string line;
    std::getline(file, line); // Skip header line
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string dateStr, valueStr;
        if (std::getline(ss, dateStr, ',') && std::getline(ss, valueStr))
        {
            Date date(dateStr);
            char* end;
            double value = strtod(valueStr.c_str(), &end);
            if (end != valueStr.c_str() && *end == '\0')
            {
                dataMap[date] = value;
            }
        }
    }
    file.close();
    return dataMap;
}

bool isValidDate(int year, int month, int day)
{
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;
    
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    
    if (month == 2)
    {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeap)
            daysInMonth[1] = 29;
    }
    
    return day <= daysInMonth[month - 1];
}

bool parseDateAndValue(const std::string& line, Date& date, double& value, std::string& error)
{
    // Skip header line
    if (line.find("date") != std::string::npos && line.find("value") != std::string::npos)
        return false;
    
    std::istringstream ss(line);
    std::string dateStr, valueStr;
    
    if (!std::getline(ss, dateStr, '|'))
    {
        error = "Error: bad input => " + line;
        return false;
    }
    
    if (!std::getline(ss, valueStr))
    {
        error = "Error: bad input => " + line;
        return false;
    }
    
    size_t start = dateStr.find_first_not_of(" \t\n\r");
    size_t end_pos = dateStr.find_last_not_of(" \t\n\r");
    if (start != std::string::npos && end_pos != std::string::npos)
        dateStr = dateStr.substr(start, end_pos - start + 1);
    else
    {
        error = "Error: bad input => " + line;
        return false;
    }
    
    start = valueStr.find_first_not_of(" \t\n\r");
    end_pos = valueStr.find_last_not_of(" \t\n\r");
    if (start != std::string::npos && end_pos != std::string::npos)
        valueStr = valueStr.substr(start, end_pos - start + 1);
    else
    {
        error = "Error: bad input => " + line;
        return false;
    }
    
    
    int year, month, day;
    if (sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day) != 3)
    {
        error = "Error: bad input => " + line;
        return false;
    }
    
    
    if (!isValidDate(year, month, day))
    {
        error = "Error: bad input => " + line;
        return false;
    }
    
    date.setYear(year);
    date.setMonth(month);
    date.setDay(day);
    
    
    char* end;
    value = strtod(valueStr.c_str(), &end);
    
    if (end == valueStr.c_str() || *end != '\0')
    {
        error = "Error: bad input => " + line;
        return false;
    }
    

    if (value < 0)
    {
        error = "Error: not a positive number.";
        return false;
    }
    
    if (value > 1000)
    {
        error = "Error: too large a number.";
        return false;
    }
    
    return true;
}

void processInputFile(const std::string& inputFilename, const std::map<Date, double>& dataMap)
{
    std::ifstream file(inputFilename.c_str());
    if (!file.is_open())
    {
        std::cerr << RED << "Error: could not open file." << RESET << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line))
    {
        Date date;
        double amount;
        std::string error;
        
        if (!parseDateAndValue(line, date, amount, error))
        {
            if (!error.empty())
                std::cerr << RED << error << RESET << std::endl;
            continue;
        }
        
        
        std::map<Date, double>::const_iterator it = dataMap.lower_bound(date);
        
        if (it != dataMap.end() && it->first == date)
        {
            
            double rate = it->second;
            std::cout << date << " => " << amount << " = " << (amount * rate) << std::endl;
        }
        else if (it != dataMap.begin())
        {
            
            --it;
            double rate = it->second;
            std::cout << date << " => " << amount << " = " << (amount * rate) << std::endl;
        }
        else
        {
            std::cerr << RED << "Error: no data available for date " << date << RESET << std::endl;
        }
    }
    
    file.close();
}

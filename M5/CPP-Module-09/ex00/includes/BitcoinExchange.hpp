#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <string>
#include <map>
#include "Colors.hpp"

class Date
{
    private:
        int year;
        int month;
        int day;

    public:
        Date();
        Date(const std::string& dateStr);
        Date(const Date& other);
        ~Date();
        
        Date& operator=(const Date& other);
        bool operator<(const Date& other) const;
        bool operator==(const Date& other) const;
        
        int getYear() const;
        int getMonth() const;
        int getDay() const;
        void setYear(int y);
        void setMonth(int m);
        void setDay(int d);
};
std::ostream& operator<<(std::ostream& os, const Date& date);
std::map<Date, double> loadDataFromCSV(const std::string& filename);
void processInputFile(const std::string& inputFilename, const std::map<Date, double>& dataMap);
bool isValidDate(int year, int month, int day);
bool parseDateAndValue(const std::string& line, Date& date, double& value, std::string& error);
#endif
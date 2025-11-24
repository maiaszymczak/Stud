#include "BitcoinExchange.hpp"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << RED << "Error: could not open file." << RESET << std::endl;
        return 1;
    }

    const std::string dataFilename = "data.csv";
    const std::string inputFilename = argv[1];

    std::map<Date, double> dataMap = loadDataFromCSV(dataFilename);
    if (dataMap.empty())
    {
        return 1;
    }

    processInputFile(inputFilename, dataMap);

    return 0;
}

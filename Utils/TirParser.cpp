#include "TirParser.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace TirParser {
bool parseTirFile(const std::string filePath, std::unordered_map<std::string, double>& coefficients) {

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return false;
    }

    std::cout << "File " << filePath << " opened successfully" << std::endl;
    std::cout << "Reading file contents..." << std::endl;
    std::cout << "-----------------------------------------------------------------" << std::endl;

    std::string line;

    int lineCount = 0;

    while (std::getline(file, line)) {
        lineCount++;

        if (line.empty()) {
            std::cout << "Line " << lineCount << " is empty. Skipping..." << std::endl;
            continue;
        }

        std::stringstream ss(line);
        std::string key, val;

        if (std::getline(ss, key,',') && std::getline(ss,val)) {
            std::cout << "Line " << lineCount << ": Key=" << key << ", Value=" << val << std::endl;
            double test = std::stod(val);

            coefficients[key] = test;

        } else {
            std::cout << "Line " << lineCount << " is invalid. Skipping..." << std::endl;
        }
    }

    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << "File " << filePath << " read successfully" << std::endl;

    file.close();
    return true;
}

}

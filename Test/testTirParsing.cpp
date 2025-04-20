#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

int main(int argc, char* argv[]) {
    std::unordered_map<std::string, double> coefficients;

    std::string filename = argv[1];

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    std::cout << "File " << filename << " opened successfully" << std::endl;
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

    std::cout << coefficients["ASS"] << std::endl;

    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << "File " << filename << " read successfully" << std::endl;

    file.close();
}

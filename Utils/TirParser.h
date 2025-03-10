#pragma once
#include <string>
#include <unordered_map>

namespace TirParser {
bool parseTirFile(const std::string filePath, std::unordered_map<std::string, double>& coefficients);
}

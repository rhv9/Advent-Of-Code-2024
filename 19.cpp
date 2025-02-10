#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <print>
#include <vector>
#include <algorithm>
#include <stack>
#include <regex>
#include <optional>
#include <chrono>
#include <cstdint>
#include <math.h>

std::vector<std::string> patterns;

bool match(const std::string& design, int offset)
{
    if (offset >= design.length())
        return true;

    for (const std::string& pattern : patterns)
    {
        bool matches = true;
        for (int i = 0; matches && i < pattern.size(); ++i)
            if (design[offset + i] != pattern[i])
                matches = false;
        
        if (matches)
            if (match(design, offset + pattern.size()))
                return true;
    }
    return false;
}

int main()
{
    auto start = std::chrono::system_clock::now();

    std::ifstream file("./input/19.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();
    int sum = 0;

    int next = input.find("\n", 0), last = 0;
    std::string patternsString = input.substr(last, next - last);
    last = next + 1;

    int nextP = 0, lastP = 0;
    while ((nextP = patternsString.find(",", lastP)) != std::string::npos)
    {
        std::string pattern = patternsString.substr(lastP, nextP - lastP);
        // cool left trimmer, could have just offset start position by 1 but this is cooler.
        pattern.erase(pattern.begin(), std::find_if(pattern.begin(), pattern.end(), [](unsigned char c) {
            return !std::isspace(c);
        }));
        lastP = nextP + 1;

        patterns.push_back(pattern);
    }

    patterns.push_back(patternsString.substr(lastP+1));

    int i = 0;
    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;
        if (line.length() == 0)
            continue;
        if (match(line, 0))
            ++sum;

        std::println("Done {}...", ++i);
    }

    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
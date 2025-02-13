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
#include <unordered_map>

std::unordered_map<std::string, bool> patternMap;
std::vector<std::string> parts;

bool match(const std::string& design, int offset, bool notPossible[100], bool matched[100], int& count)
{   
    //std::println("Offset {} Need: {}", offset, design.substr(offset, design.size() - offset));
    if (offset >= design.length())
    {
        count++;
        return true;
    }
    if (notPossible[offset])
        return false;
    
    if (matched[offset])
        return true;
        
    for (int i = 1; i < design.size() - offset + 1; ++i)
    {
        std::string test = design.substr(offset, i);
        //std::println("Attempt: {}", test);
        if (patternMap.contains(test))
        {   
            parts.push_back(test);
            //std::println("Pattern found: {}", test);
            if (match(design, offset + i, notPossible, matched, count))
            {
                matched[offset] = true;
                if (offset != 0)
                    return true;
            }
            parts.pop_back();
        }
    }
    if (!matched[offset])
        notPossible[offset] = true;

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
        
        patternMap[pattern] = true;
    }   

    patternMap[patternsString.substr(lastP+1)] = true;

    int i = 0;
    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;
        if (line.length() == 0)
            continue;
        std::println("Design: {}", line);
        bool notPossible[100] = {false};
        bool matched[100] = {false};

        int count = 0;

        match(line, 0, notPossible, matched, count);
        sum += count;
        

        std::println("> Done {}...", ++i);
        for (auto part : parts)
            std::print("{} + ", part);

        std::println("\n Design length: {}", line.length());

        parts = {};
        //std::cin.get();
    }

    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
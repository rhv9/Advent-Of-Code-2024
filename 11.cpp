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

std::optional<uint64_t> getNumber(const std::string &input, int &pointer)
{
    int initialPointer = pointer;
    while (pointer >= 0 && pointer < input.size() &&
           input[pointer] >= int('0') && input[pointer] <= int('9'))
    {
        ++pointer;
    }

    std::string numString = input.substr(initialPointer, pointer);
    return !numString.length() ? std::optional<uint64_t>{} : std::stol(numString);
}

int main()
{
    auto start = std::chrono::system_clock::now();

    std::ifstream file("./input/11.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();

    int sum = 0;
    
    std::vector<uint64_t> numbers;
    int pointer = 0;
    while (pointer < input.size())
    {
        int num = getNumber(input, pointer).value();
        ++pointer;
        numbers.push_back(num);
    }

    for (int count = 0; count < 25; ++count)
    {
        int size = numbers.size();
        for (int i = 0; i < size; ++i)
        {
            uint64_t num = numbers[i];
            std::string numString;
            if (num == 0)
                numbers[i] = 1;
            else if ((numString = std::to_string(num)).size() % 2 == 0)
            {   
                uint64_t left = std::stoll(numString.substr(0, numString.size()/2));
                uint64_t right = std::stoll(numString.substr(numString.size()/2, numString.size()/2));

                numbers[i] = left;
                numbers.push_back(right);
            }
            else
                numbers[i] *= 2024;
        }
        
    }

    sum = numbers.size();

    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
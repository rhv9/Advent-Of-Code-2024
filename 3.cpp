#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <print>
#include <vector>
#include <algorithm>
#include <stack>
#include <regex>
#include <optional>


std::optional<int> getNumber(const std::string& input, int& pointer)
{
    std::stack<int> numStack;

    while (pointer < input.length() && input[pointer] >= '0' && input[pointer] <= '9')
    {
        numStack.push(input[pointer] - int('0'));
        ++pointer;
    }

    if (numStack.empty())
        return {};

    int num = 0;
    int digit = 1;
    while (!numStack.empty())
    {
        num += digit * numStack.top();
        digit *= 10;
        numStack.pop();
    }

    return num;
}

int main()
{
    std::ifstream file("./input/3.txt");
    std::stringstream buffer; 
    buffer << file.rdbuf();

    std::string input = buffer.str();

    int sum = 0;
    size_t last = 0;
    size_t next = 0;
    while ((next = input.find("mul(", last)) != std::string::npos)
    {
        int pointer = next + std::string("mul(").length();
        std::string line = input.substr(pointer, 10);
        last = next + 1;

        std::optional<int> first = getNumber(input, pointer);
        
        if (pointer >= input.length() || input[pointer] != ',')
            continue;
        
        ++pointer;

        std::optional<int> second = getNumber(input, pointer); 

        if (pointer >= input.length() || input[pointer] != ')')
            continue;
        
        sum += first.value_or(0) * second.value_or(0);
    }

    std::print("{}\n", sum);

}
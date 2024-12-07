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
#include <chrono>

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
    auto start = std::chrono::system_clock::now();
    
    std::ifstream file("./input/5.txt");
    std::stringstream buffer; 
    buffer << file.rdbuf();

    std::string input = buffer.str();

    std::vector<std::string> inputs;
    std::vector<std::vector<int>> updates;

    int sum = 0;
    size_t last = 0;
    size_t next = 0;
    bool rules = true;

    int ordering[100][100] = {0};

    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;

        if (line.size() == 0)
        {
            rules = false;
            continue;
        }

        if (rules)
        {
            int pointer = 0;
            int first = getNumber(line, pointer).value_or(0);
            ++pointer;
            int second = getNumber(line, pointer).value_or(0);
            ordering[first][second] = -1; // left
            ordering[second][first] =  1; // right
        }
        else
        {
            int pointer = 0;
            std::vector<int> nums;

            while (pointer < line.size())
            {
                std::optional<int> num = getNumber(line, pointer);
                ++pointer;
                if (num.has_value())
                    nums.push_back(num.value());
            }

            updates.push_back(nums);
        }

        inputs.push_back(line);
    }

    for (const std::vector<int>& nums : updates)
    {
        bool found = true;

        for (int i = 0; i < nums.size()-1 && found; i++)
        {
            if (ordering[nums[i]][nums[i+1]] == 1)
                found = false;
        }

        if (found)
            sum += nums[nums.size()/2];
    }

    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);

}
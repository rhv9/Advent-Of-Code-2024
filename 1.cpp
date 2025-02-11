#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <print>
#include <vector>
#include <algorithm>

int main()
{
    std::ifstream file("./input/1.txt");
    std::stringstream buffer; 
    buffer << file.rdbuf();

    std::vector<int> firstList;
    firstList.reserve(1000);
    std::vector<int> secondList;
    secondList.reserve(1000);

    std::string input = buffer.str();

    size_t last = 0;
    size_t next = 0;
    while ((next = input.find('\n', last)) != std::string::npos)
    {
        std::string line = input.substr(last, next-last);
        last = next + 1;

        int first = std::stoi(line.substr(0, 5));
        int second = std::stoi(line.substr(8));

        firstList.emplace_back(first);
        secondList.emplace_back(second);
    }

    std::sort(firstList.begin(), firstList.end());
    std::sort(secondList.begin(), secondList.end());

    int sum = 0;
    for (int i = 0; i < firstList.size(); i++)
    {
        int diff = firstList[i] - secondList[i];
        sum += std::abs(diff);
    }

    std::print("{}\n", sum);

}
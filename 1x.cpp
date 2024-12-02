#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <print>
#include <vector>
#include <algorithm>

//18567089
int main()
{
    std::ifstream file("./input/1.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::vector<int> firstList;
    firstList.reserve(1000);

    std::string input = buffer.str();

    int hash[99999] = {0};
    size_t last = 0;
    size_t next = 0;
    while ((next = input.find('\n', last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;

        int first = std::stoi(line.substr(0, 5));
        int second = std::stoi(line.substr(8));

        firstList.emplace_back(first);
        hash[second]+= second;
    }

    int sum;

    for (int num : firstList)
        sum += hash[num];

    std::print("{}\n", sum);
}
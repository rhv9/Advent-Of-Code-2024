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

#define size_int uint64_t

std::optional<size_int> getNumber(const std::string& input, int& pointer)
{
    int initialPointer = pointer;
    while (pointer >= 0 && pointer <= input.length() && 
            input[pointer] >= '0' && input[pointer] <= '9')
        ++pointer;

    // potential error here if size was long size
    return (pointer == initialPointer) ? std::optional<size_int>{} : std::stoi(input.substr(initialPointer, pointer - initialPointer));
}   


uint64_t mix(uint64_t a, uint64_t b)
{
    return a ^ b;
}

uint64_t prune(uint64_t x)
{
    return x % 16777216;
}

uint64_t nextSecret(uint64_t secret)
{
    secret = prune(mix(secret, secret * 64));
    secret = prune(mix(secret, (uint64_t)(secret / 32)));
    secret = prune(mix(secret, secret * 2048));

    return secret;
}

int main()
{
    auto start = std::chrono::system_clock::now();

    std::ifstream file("./input/22.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();

    uint64_t sum = 0;
    int next = 0, last = 0;
    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;
        //std::print("{}: ", line);

        uint64_t secret = std::stoi(line);


        for (int i = 0; i < 2000; ++i)
            secret = nextSecret(secret);

        //std::println("{}", secret);
        sum += secret;
    }

    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
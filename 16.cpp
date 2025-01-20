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

struct Pos
{
    Pos(int X, int Y) : x(X), y(Y) {}
    Pos() : x(0), y(0) {}

    bool operator==(const Pos& right) const
    {
        return x == right.x && y == right.y;
    }

    Pos operator+(const Pos& right) const
    {
        return {x + right.x, y + right.y};
    }

    const std::string toString() const
    {
        std::stringstream ss;
        ss << "(" << x << "," << y << ")";
        return ss.str(); 
    } 

    int x, y;
};

int main()
{
    auto start = std::chrono::system_clock::now();

    std::ifstream file("./input/15.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();

    uint64_t sum = 0;
    size_t last = 0;
    size_t next = 0;

    // generate map
    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;

    } 

    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
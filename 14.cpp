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

std::optional<int32_t> getNumber(const std::string &input, int &pointer)
{
    int initialPointer = pointer;
    if (pointer >= 0 && pointer < input.size() && input[pointer] == '-')
        ++pointer;

    while (pointer >= 0 && pointer < input.size() &&
           input[pointer] >= int('0') && input[pointer] <= int('9'))
        ++pointer;

    std::string numString = input.substr(initialPointer, pointer);
    if (numString == std::string("-"))
        return std::optional<int32_t>{};

    return !numString.length() ? std::optional<int32_t>{} : std::stoi(numString);
}

// thanks stackoverflow, what will I ever do without you my beloved <3
int wrapAroundMod(int a, int n) {
    int offsetTimes = 0;

    if (a < 0)
        offsetTimes = (-a / n) + 1;

    return (a + n * offsetTimes) % n;
}

int main()
{
    auto start = std::chrono::system_clock::now();

    std::ifstream file("./input/14.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();

    int quadrant[2][2] = {0};
    int wide = 101, tall = 103;
    int runs = 100;
    int midX = wide / 2;
    int midY = tall / 2;


    uint64_t sum = 0;
    size_t last = 0;
    size_t next = 0;
    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;
        
        Pos p, v;

        int pointer = std::string("p=").length();
        p.x = getNumber(line, pointer).value();
        ++pointer;
        p.y = getNumber(line, pointer).value();

        pointer += std::string(" v=").length();
        v.x = getNumber(line, pointer).value();
        ++pointer;
        v.y = getNumber(line, pointer).value();

        Pos newPos;
        newPos.x = wrapAroundMod(p.x + v.x * runs, wide);
        newPos.y = wrapAroundMod(p.y + v.y * runs, tall);

        if (newPos.x == midX || newPos.y == midY)
            continue;

        Pos quadrantPos {newPos.x / (midX+1), newPos.y / (midY+1)};
        ++quadrant[quadrantPos.y][quadrantPos.x];
    } 
    sum = 1;

    for (int y = 0; y < 2; y++)
        for (int x = 0; x < 2; x++)
            sum *= quadrant[y][x];


    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
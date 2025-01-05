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
    Pos(float X, float Y) : x(X), y(Y) {}
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

    float x, y;
};

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

    std::ifstream file("./input/13.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();

    std::vector<std::string> buttonLines;
    uint64_t sum = 0;
    size_t last = 0;
    size_t next = 0;
    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;
        
        if (line.length() < 2)
            continue;
        else if (line[0] == 'B')
            buttonLines.push_back(line);
        else if (line[0] == 'P')
        {
            if (buttonLines.size() != 2)
                std::println("There are not 2 buttons recorded, something is wrong");
            int pointer = 0;
            // Button A
            Pos a, b, p;
            pointer += std::string("Button A: X+").length();
            a.x = getNumber(buttonLines[0], pointer).value();
            pointer += std::string(", Y+").length();
            a.y = getNumber(buttonLines[0], pointer).value();

            pointer = 0;
            pointer += std::string("Button B: X+").length();
            b.x = getNumber(buttonLines[1], pointer).value();
            pointer += std::string(", Y+").length();
            b.y = getNumber(buttonLines[1], pointer).value();

            pointer = 0;
            pointer += std::string("Prize: X=").length();
            p.x = getNumber(line, pointer).value();
            pointer += std::string(", Y=").length();
            p.y = getNumber(line, pointer).value();

            buttonLines.clear();
            //std::println("Pos A: {}, Pos B: {}", a.toString(), b.toString());
            //std::println("Prize: {}", p.toString());

            float buttonA = ((p.y * b.x) - (p.x * b.y) ) / ((a.y * b.x) - (a.x * b.y));
            float buttonB = ((p.y * a.x) - (p.x * a.y) ) / ((b.y * a.x) - (b.x * a.y));

            //std::println("Button A: {} Button B: {}", buttonA, buttonB);

            if (buttonA < 0 || buttonA > 100 || buttonB < 0 || buttonB > 100 ||
                std::ceilf(buttonA) != buttonA || std::ceilf(buttonB) != buttonB)
                continue;

            int tokens = buttonA * 3 + buttonB;
            sum += tokens;
            //std::println("Tokens: {}", tokens);
        }
        else 
            std::println("This should not run, please investigate.");


    }


    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
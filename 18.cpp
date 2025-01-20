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

#define TEST
#define size_int uint8_t

#ifdef TEST
    #define PRINT(x,...) std::println(x,__VA_ARGS__)
#else
    #define PRINT(x,...)
#endif

struct Vec2
{
    Vec2(int X, int Y) : x(X), y(Y) {}
    Vec2() : x(0), y(0) {}

    bool operator==(const Vec2& right) const
    {
        return x == right.x && y == right.y;
    }

    Vec2 operator+(const Vec2& right) const
    {
        return {x + right.x, y + right.y};
    }

    Vec2 negativeSign() const
    {
        return Vec2{-x, -y};
    }

    const std::string toString() const
    {
        std::stringstream ss;
        ss << "(" << x << "," << y << ")";
        return ss.str(); 
    } 

    int x, y;
};

std::optional<size_int> getNumber(const std::string& input, int& pointer)
{
    int initialPointer = pointer;
    while (pointer >= 0 && pointer <= input.length() && 
            input[pointer] >= '0' && input[pointer] <= '9')
        ++pointer;

    // potential error here if size was long size
    return (pointer == initialPointer) ? std::optional<size_int>{} : std::stoi(input.substr(initialPointer, pointer - initialPointer));
}   

constexpr int MEMORY_SIZE = 71;
constexpr int MAX_BYTE_DROP = 1024;
int byteCount = 0;
Vec2 dirs[4] =
{
    Vec2 { 0, -1},
    Vec2 { 0,  1},
    Vec2 {-1,  0},
    Vec2 { 1,  0},
};

std::array<std::array<bool, MEMORY_SIZE>, MEMORY_SIZE> canWalk;
std::array<std::array<bool, MEMORY_SIZE>, MEMORY_SIZE> visited;
std::array<std::array<int, MEMORY_SIZE>, MEMORY_SIZE> cost;
std::array<std::array<Vec2, MEMORY_SIZE>, MEMORY_SIZE> previousStep;
Vec2 target {MEMORY_SIZE-1, MEMORY_SIZE-1};

void visit(const Vec2& p)
{
    visited[p.y][p.x] = true;

    for (const Vec2& dir : dirs)
    {
        Vec2 walkPos = p + dir;

        if (walkPos.y < 0 || walkPos.y >= MEMORY_SIZE ||
            walkPos.x < 0 || walkPos.x >= MEMORY_SIZE || !canWalk[walkPos.y][walkPos.x])
            continue;
        
        int currentCost = cost[p.y][p.x];
        if (!visited[walkPos.y][walkPos.x])
        {
            cost[walkPos.y][walkPos.x] = currentCost + 1;
            previousStep[walkPos.y][walkPos.x] = dir.negativeSign();
            visit(walkPos);
        }
        else
        {
            int bestCost =  cost[walkPos.y][walkPos.x];
            if (currentCost + 1 < bestCost)
            {
                cost[walkPos.y][walkPos.x] = currentCost + 1;
                previousStep[walkPos.y][walkPos.x] = dir.negativeSign();
                visit(walkPos);
            }
        }
    }   
}

int main()
{
    auto start = std::chrono::system_clock::now();

    std::ifstream file("./input/18.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();

    for (int y = 0; y < MEMORY_SIZE; ++y)
    {
        for (int x = 0; x < MEMORY_SIZE; ++x)
        {
            canWalk[y][x] = true;
            visited[y][x] = false;
        }
    }

    int sum = 0;
    int next = 0, last = 0;
    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;

        int pointer = 0;
        size_int x = getNumber(line, pointer).value();
        ++pointer;
        size_int y = getNumber(line, pointer).value();
        canWalk[y][x] = false;

        if (++byteCount >= MAX_BYTE_DROP)
            break;
    }

    visit(Vec2{0,0});
    sum = cost[MEMORY_SIZE-1][MEMORY_SIZE-1];
    
    // Create map string
    //std::vector<std::string> charMap;
    //for (auto array : canWalk)
    //{
    //    std::string xMap;
    //    for (auto i : array)
    //        xMap.append((i) ? "." : "#");
    //    charMap.push_back(xMap);
    //}
    // Print map
    //for (auto str : charMap)
    //    std::println("{}", str);

    // create map with path followed
    //std::vector<std::string> pathCharMap = charMap;
    //Vec2 currentPos {MEMORY_SIZE-1, MEMORY_SIZE-1};
    //while (currentPos != Vec2{0, 0})
    //{
    //    currentPos = currentPos + previousStep[currentPos.y][currentPos.x];
    //    pathCharMap[currentPos.y][currentPos.x] = 'O';
    //    //std::print("{}, ", cost[currentPos.y][currentPos.x]);
    //}
    //std::println("\n");
    //for (auto str : pathCharMap)
    //    std::println("{}", str);

    // Print visited parts of map
    //std::vector<std::string> visitedCharMap = charMap;
    //for (int y = 0; y < MEMORY_SIZE; ++y)
    //{
    //    for (int x = 0; x < MEMORY_SIZE; ++x)
    //    {
    //        if (visited[y][x])
    //            visitedCharMap[y][x] = '/';
    //    }
    //}
    //std::println("");
    //for (auto str : visitedCharMap)
    //    std::println("{}", str);


    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
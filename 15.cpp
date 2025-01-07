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

std::vector<std::string> map;
Pos robotPos;

bool move(const Pos& pMove, Pos p)
{
    char tile = map[p.y][p.x];
    Pos newP = pMove + p;

    if (tile == '.') 
        return true;
    if (tile == '#')
        return false;
    if (tile == 'O')
    {
        if (move(pMove, newP))
        {
            map[newP.y][newP.x] = 'O';
            map[p.y][p.x] = '.';
            return true;
        }
        return false;
    }
    if (tile == '@')
    {
        if (move(pMove, newP))
        {
            map[newP.y][newP.x] = '@';
            map[p.y][p.x] = '.';
            robotPos = newP;
            return true;
        }
        return false;
    }
    return false;
}


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

        if (line.length() == 0)
            break;
        
        if (robotPos == Pos{ 0, 0 })
            for (int x = 0; x < line.length(); ++x)
                if (line[x] == '@')
                    robotPos = {x, (int)map.size()};

        map.push_back(line);
    } 

    // robot likes to move it move it..
    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;   

        for (char dir : line)
        {
            switch (dir)
            {
                case '<':
                    move(Pos{-1, 0}, robotPos);
                    break;
                case '>':
                    move(Pos{1, 0}, robotPos);
                    break;
                case '^':
                    move(Pos{0, -1}, robotPos);
                    break;
                case 'v':
                    move(Pos{0, 1}, robotPos);
                    break;
                default:
                    std::println("This should never ever run!, robot cannot move in dir {}", dir);
            }
        }
    } 

    for (int y = 0; y < map.size(); ++y)
    {
        for (int x = 0; x < map.size(); ++x)
        {
            if (map[y][x] == 'O')
                sum += 100 * y + x;
        }
    }

    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
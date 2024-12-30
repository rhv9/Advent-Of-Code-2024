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

struct Pos
{
    Pos(int X, int Y) : x(X), y(Y) {}
    Pos() : x(0), y(0) {}

    bool operator==(const Pos& right) const
    {
        return x == right.x && y == right.y;
    }

    const std::string toString() const
    {
        std::stringstream ss;
        ss << "(" << x << "," << y << ")";
        return ss.str(); 
    } 

    int x, y;
};

int recurseClimb(std::vector<std::string> map, Pos pos, int nextTrail, std::vector<Pos>& trailHeadsFound)
{
    if (map[pos.y][pos.x] == '9')
    {
        for (Pos& p : trailHeadsFound)
        {
            if (pos == p)
                return 0;
        }
        trailHeadsFound.push_back(pos);
        return 1;
    }

    Pos dirs[] = 
    {
        Pos{ 0, -1},
        Pos{ 0,  1},
        Pos{-1,  0},
        Pos{ 1,  0},
    };
    int sum = 0;
    for (Pos& dir : dirs)
    {
        Pos p {pos.x + dir.x, pos.y + dir.y};
        
        if (p.y < 0 || p.y >= map.size() || p.x < 0 || p.x >= map[p.y].size())
            continue;
        
        if (map[p.y][p.x] - '0' == nextTrail)
            sum += recurseClimb(map, p, nextTrail+1, trailHeadsFound);
    }

    return sum;
}

int main()
{
    auto start = std::chrono::system_clock::now();

    std::ifstream file("./input/10.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();
    std::vector<std::string> map;

    uint64_t sum = 0;
    size_t last = 0;
    size_t next = 0;
    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;
        map.push_back(line);
    }


    for (int y = 0; y < map.size(); ++y)
    {
        for (int x = 0; x < map[y].size(); ++x)
        {
            if (map[y][x] == '0')
            {   
                std::vector<Pos> trailHeadsFound;
                sum += recurseClimb(map, Pos{x, y}, 1, trailHeadsFound);
            }
        }
    }

    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
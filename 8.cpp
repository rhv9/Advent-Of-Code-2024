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
#include <cstdint>
#include <unordered_map>
#include <unordered_set>

struct Pos
{
    Pos(int x, int y) : x(x), y(y) {}
    int x, y;

    Pos operator-(const Pos& right)
    {
        return {this->x - right.x, this->y - right.y};
    }

    Pos operator+(const Pos& right)
    {
        return {this->x + right.x, this->y + right.y};
    }
    
    void print()
    {
        std::print("({},{})", x, y);
    }

    void println()
    {
        std::print("({},{})\n", x, y);
    }
};


int main()
{
    auto start = std::chrono::system_clock::now();

    std::ifstream file("./input/8.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();

    uint64_t sum = 0;
    size_t last = 0;
    size_t next = 0;
    int y = 0;
    std::unordered_map<char, std::vector<Pos>> map;
    std::unordered_set<char> keys;
    std::vector<std::string> lines;
    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        lines.push_back(line);
        last = next + 1;

        for (int x = 0; x < line.size(); x++)
        {
            char c = line[x];

            if (c != '.')
            {
                map[c].push_back({x, y});
                keys.emplace(c);
            }
        }
        ++y;
    }

    bool counted[lines.size()][lines[0].size()] = {{false}};
    for (std::unordered_map<char, std::vector<Pos>>::iterator it = map.begin(); it != map.end(); ++it)
    {
        std::println("==={}===", it->first);
        std::vector<Pos>& list = map[it->first];
        for (int i = 0; i < list.size(); ++i)
        {
            for (int j = i+1; j < list.size(); ++j)
            {
                list[i].println();
                list[j].println();
                Pos diff = list[j] - list[i];
                std::print("Diff ");
                diff.println();
                Pos pos = list[i] - diff;

                for (int loop = 0; loop < 2; ++loop)
                {
                    std::print("Pos {}: ", loop+1);
                    pos.println();
                    if (pos.y < 0 || pos.y >= lines.size() ||
                        pos.x < 0 || pos.x >= lines[pos.y].size())
                    {
                        pos = list[j] + diff;
                        continue;
                    }
                    std::println(" > char: {}", lines[pos.y][pos.x]);
                    std::cin.get();
                    if (lines[pos.y][pos.x] == '.')
                    {
                        std::println("!Added");
                        ++sum;
                        lines[pos.y][pos.x] = '#';
                    }
                    else if (lines[pos.y][pos.x] != '#' && !counted[pos.y][pos.x])
                    {
                        std::println("!Added as not counted");
                        ++sum;
                        counted[pos.x][pos.y] = true;
                    }
                    pos = list[j] + diff;
                }

            }
        }
    }

    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
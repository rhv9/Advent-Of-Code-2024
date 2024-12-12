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

// up    0 -1
// right 1  0
// down  0  1
// left -1  0
int main()
{
    auto start = std::chrono::system_clock::now();

    std::ifstream file("./input/6.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();

    std::vector<std::string> map;

    int sum = 0;
    size_t last = 0;
    size_t next = 0;
    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;

        map.push_back(line);
    }

    // find guard position
    int x = 0;
    int y = -1;
    for (int i = 0; i < map.size() && y == -1; i++)
        if ((x = map[i].find('^')) != std::string::npos)
            y = i;

    map[y][x] = '.';
    int array[] = {-1, 0, 1, 0};

    int i = 1;
    int xmove = array[i % 4];
    int ymove = array[(i - 1) % 4];
    while (y >= 0 && y < map.size() &&
           x >= 0 && x < map[y].length())
    {
        if (map[y][x] == '.')
            ++sum;

        else if (map[y][x] == '#')
        {
            x -= xmove;
            y -= ymove;
            ++i;
            xmove = array[i % 4];
            ymove = array[(i - 1) % 4];
        }

        map[y][x] = 'X';
        x += xmove;
        y += ymove;
    }

    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
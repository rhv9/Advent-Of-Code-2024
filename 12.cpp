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


struct Pos;

constexpr int VISITED_SIZE = 400;
std::vector<std::string> map;
bool visited[VISITED_SIZE][VISITED_SIZE] = {false};
std::vector<Pos> toVisitList;

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

void calculatePerimeter(int& perimeter, int& area, Pos p)
{
    char crop = map[p.y][p.x];
    visited[p.y][p.x] = true;
    ++area;

    Pos dirs[] = { Pos{0, -1}, Pos{0, 1}, Pos{-1, 0}, Pos{1, 0} };
    for (Pos& dir : dirs)
    {
        Pos np = p + dir;
        if (np.y < 0 || np.y >= map.size() || np.x < 0 || np.x >= map[np.y].size())
            ++perimeter;
        else if (crop !=  map[np.y][np.x])
        {
            ++perimeter;
            if (!visited[np.y][np.x])
                toVisitList.push_back(np);
        }
        else if (!visited[np.y][np.x])
            calculatePerimeter(perimeter, area, np);
    }
    return;
}

int main()
{
    auto start = std::chrono::system_clock::now();

    std::ifstream file("./input/12.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();

    uint64_t sum = 0;
    size_t last = 0;
    size_t next = 0;
    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;
        map.push_back(line);
    }

    toVisitList.push_back(Pos{0, 0});

    int max = 0;
    int iterations = 0;
    while (!toVisitList.empty())
    {
        ++iterations;
        max = toVisitList.size() > max ? toVisitList.size() : max;
        Pos& p = toVisitList.back();

        if (visited[p.y][p.x])
            toVisitList.pop_back();
        else
        {
            int perimeter = 0;
            int area = 0;

            calculatePerimeter(perimeter, area, p);
            sum += area * perimeter;
        }
    }

    std::println("Max visit list is {}", max);
    std::println("Iterations for solution is {}", iterations);
    std::println("Map area is {}", map.size() * map[0].size());
    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
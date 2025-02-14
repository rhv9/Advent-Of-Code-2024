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
#include <unordered_map>

#define size_int uint8_t

struct Vec2
{
    Vec2(int X, int Y) : x(X), y(Y) {}
    Vec2() : x(0), y(0) {}

    bool operator==(const Vec2 &right) const
    {
        return x == right.x && y == right.y;
    }

    Vec2 operator+(const Vec2 &right) const
    {
        return {x + right.x, y + right.y};
    }

    Vec2 negativeSign() const
    {
        return Vec2{-x, -y};
    }

    char toDirChar() const
    {
        if (y == 0 && x < 0)
            return '<';
        if (y == 0 && x > 0)
            return '>';
        if (x == 0 && y < 0)
            return '^';
        if (x == 0 && y > 0)
            return 'v';

        return '?';
    }

    const std::string toString() const
    {
        std::stringstream ss;
        ss << "(" << x << "," << y << ")";
        return ss.str();
    }

    int x, y;
};

void printVector(const std::vector<std::string> &vec)
{
    std::print("[");
    for (auto i : vec)
        std::print("{}, ", i);
    std::println("]");
}

constexpr int MAXSIZE = 10;
Vec2 dirs[4] =
    {
        Vec2{0, -1},
        Vec2{0, 1},
        Vec2{-1, 0},
        Vec2{1, 0},
};

void visit(const Vec2 &p,
           const std::vector<std::string> &buttons,
           bool visited[MAXSIZE][MAXSIZE],
           int cost[MAXSIZE][MAXSIZE],
           Vec2 previousStep[MAXSIZE][MAXSIZE])
{
    visited[p.y][p.x] = true;

    for (const Vec2 &dir : dirs)
    {
        Vec2 walkPos = p + dir;

        if (walkPos.y < 0 || walkPos.y >= buttons.size() ||
            walkPos.x < 0 || walkPos.x >= buttons[walkPos.y].size() || buttons[walkPos.y][walkPos.x] == '.')
            continue;

        int currentCost = cost[p.y][p.x];
        if (!visited[walkPos.y][walkPos.x])
        {
            cost[walkPos.y][walkPos.x] = currentCost + 1;
            previousStep[walkPos.y][walkPos.x] = dir.negativeSign();
            visit(walkPos, buttons, visited, cost, previousStep);
        }
        else
        {
            int bestCost = cost[walkPos.y][walkPos.x];
            if (currentCost + 1 < bestCost)
            {
                cost[walkPos.y][walkPos.x] = currentCost + 1;
                previousStep[walkPos.y][walkPos.x] = dir.negativeSign();
                visit(walkPos, buttons, visited, cost, previousStep);
            }
        }
    }
}

std::string getSteps(Vec2 previousStep[MAXSIZE][MAXSIZE], const Vec2 &start, const Vec2 &end)
{
    std::string steps = "";
    Vec2 pos = end;

    // td::println("Start: {}, end: {}", start.toString(), end.toString());
    while (!(pos == start))
    {
        Vec2 &prev = previousStep[pos.y][pos.x];
        // std::println("Pos: {}, Prev: {}", pos.toString(), prev.toString());
        // std::cin.get();
        steps.push_back(prev.negativeSign().toDirChar());
        pos = pos + prev;
    }

    std::reverse(steps.begin(), steps.end());
    // std::println("steps: {}", steps);
    // std::cin.get();
    return steps;
}

std::array<std::array<std::string, 256>, 256> generateMap(const std::vector<std::string> &buttons)
{
    std::array<std::array<std::string, 256>, 256> map;
    for (int y = 0; y < buttons.size(); ++y)
    {
        for (int x = 0; x < buttons[y].length(); ++x)
        {

            if (buttons[y][x] == '.')
                continue;
            bool visited[MAXSIZE][MAXSIZE];
            int cost[MAXSIZE][MAXSIZE];
            Vec2 previousStep[MAXSIZE][MAXSIZE];

            for (int iy = 0; iy < MAXSIZE; ++iy)
            {
                for (int ix = 0; ix < MAXSIZE; ++ix)
                {
                    visited[iy][ix] = false;
                    cost[iy][ix] = 0;
                    previousStep[iy][ix] = Vec2{0, 0};
                }
            }
            char button = buttons[y][x];
            visit({x, y}, buttons, visited, cost, previousStep);

            for (int ya = 0; ya < buttons.size(); ++ya)
            {
                for (int xa = 0; xa < buttons[y].length(); ++xa)
                {
                    if (buttons[ya][xa] == '.')
                        continue;
                    std::string steps = getSteps(previousStep, {x, y}, {xa, ya});

                    std::unordered_map<char, int> keys;
                    for (char step : steps)
                        keys[step] = keys.contains(step) ? keys[step] + 1 : 1;

                    // if starting position is left most, then prioritise moving horizontally
                    std::string priorityList("<>^vA");
                    if (xa == 0)
                        priorityList = "^v<>A";

                    std::string newSteps = "";
                    for (char c : priorityList)
                    {
                        if (keys.contains(c))
                            for (int j = 0; j < keys[c]; ++j)
                                newSteps.push_back(c);
                    }

                    // std::println("Old: {}, New: {}", steps, newSteps);
                    // std::cin.get();
                    steps = newSteps;
                    steps.push_back('A');
                    map[button][buttons[ya][xa]] = steps;
                    // std::cin.get();
                    // std::println("From {} to {} steps are {}", button, buttons[ya][xa], steps);
                }
            }
        }
    }

    return map;
}

void swap(std::string &first, std::string &second)
{
    std::string temp = first;
    first = second;
    second = temp;
}

void getPermutations(std::vector<std::string> &results, std::vector<std::string> &elements, int pos)
{
    if (pos == elements.size() - 1)
    {
        std::string newCombination;
        for (auto s : elements)
            newCombination.append(s);
        results.push_back(newCombination);
        return;
    }

    std::unordered_map<std::string, bool> foundMap;
    for (int i = pos; i < elements.size(); ++i)
    {
        if (foundMap.contains(elements[i]))
            continue;
        foundMap[elements[i]] = true;
        swap(elements[pos], elements[i]);
        getPermutations(results, elements, pos + 1);
        swap(elements[pos], elements[i]);
    }
}

bool validMove(const std::vector<std::string> &keypad, const std::string &steps, const char start, const char end, const Vec2 &startPos)
{
    Vec2 currentPos = startPos;
    for (auto step : steps)
    {
        Vec2 moveVec;
        if (step == '^')
            moveVec = {0, -1};
        else if (step == 'v')
            moveVec = {0, 1};
        else if (step == '<')
            moveVec = {-1, 0};
        else if (step == '>')
            moveVec = {1, 0};

        currentPos = currentPos + moveVec;

        if (currentPos.y < 0 || currentPos.y >= keypad.size() ||
            currentPos.x < 0 || currentPos.x >= keypad[currentPos.y].size() || keypad[currentPos.y][currentPos.x] == '.')
        {
            //std::println("Invalid move | start {}, end {}, steps {}, currentPos {}", start, end, steps, currentPos.toString());
            //std::println("StartPos {}", startPos.toString());
            // std::cin.get();
            return false;
        }
    }

    return keypad[currentPos.y][currentPos.x] == end;
}

std::array<std::array<std::vector<std::string>, 256>, 256> generateStepPermutations(const std::vector<std::string> &keypad, const std::array<std::array<std::string, 256>, 256> &stepsMap)
{
    std::array<std::array<std::vector<std::string>, 256>, 256> permutationsResult;

    for (int i = 0; i < keypad.size(); ++i)
    {
        for (int xi = 0; xi < keypad[i].size(); ++xi)
        {
            char start = keypad[i][xi];
            if (start == '.')
                continue;
            for (int j = 0; j < keypad.size(); ++j)
            {
                for (char end : keypad[j])
                {
                    if (end == '.')
                        continue;

                    if (start == end)
                    {
                        std::vector<std::string> permutations;
                        permutations.push_back("A");
                        permutationsResult[start][end] = permutations;
                        continue;
                    }
                    std::string steps = stepsMap[start][end];
                    //std::println("Start: {}, End: {}, Steps: {}", start, end, steps);
                    steps.pop_back();

                    std::unordered_map<char, int> keysMap;
                    for (char step : steps)
                        keysMap[step] = keysMap.contains(step) ? keysMap[step] + 1 : 1;

                    std::vector<std::string> pieces;
                    for (auto [key, count] : keysMap)
                    {
                        std::string toPut = "";
                        for (int iter = 0; iter < count; ++iter)
                            toPut.push_back(key);
                        pieces.push_back(toPut);
                    }

                    //for (auto str : pieces)
                    //    std::println("Piece {}", str);

                    std::vector<std::string> permutations;
                    getPermutations(permutations, pieces, 0);
                    std::transform(permutations.begin(), permutations.end(), permutations.begin(), [](std::string str)
                                   { return str + "A"; });

                    //std::print("Unfiltered permutations: [");
                    //for (auto str : permutations)
                    //    std::print("{}, ", str);
                    //std::print("]\n");
                    // std::cin.get();

                    Vec2 startPos = Vec2{xi, i};
                    std::erase_if(permutations, [keypad, start, end, startPos](const std::string &item)
                                  { return !::validMove(keypad, item, start, end, startPos); });

                    //std::print("New permutations: [");
                    //for (auto str : permutations)
                    //    std::print("{}, ", str);
                    //std::println("]");
                    // std::cin.get();
                    permutationsResult[start][end] = permutations;
                }
            }
        }
    }

    return permutationsResult;
}

void findLowestSteps(const std::array<std::array<std::vector<std::string>, 256>, 256> &permutationsMap,
                     const std::string &action, const char start,
                     const std::string &stepsDone, std::vector<std::string> &lowestSteps, int &lowestStepsSize, int ptr)
{
    if (ptr >= action.size())
    {
        // std::println("Lowest: {} Steps Done: {}", lowestSteps, stepsDone);
        // std::cin.get();
        //  first one
        if (lowestStepsSize == -1 || stepsDone.size() <= lowestStepsSize)
        {
            lowestStepsSize = stepsDone.size();

            std::erase_if(lowestSteps, [lowestStepsSize](const std::string &elem)
                          { return elem.size() > lowestStepsSize; });
            lowestSteps.push_back(stepsDone);
        }
        return;
    }

    char end = action[ptr];
    std::vector<std::string> permutations = permutationsMap[start][end];
    // std::println("Size: {}", permutations.size());
    if (permutations.size() == 0)
    {
        std::println("Start {} End {}", start, end);
        std::cin.get();
    }
    // for each possible steps
    for (auto steps : permutations)
    {
        findLowestSteps(permutationsMap, action, end, stepsDone + steps, lowestSteps, lowestStepsSize, ptr + 1);
    }
}

int main()
{
    auto start = std::chrono::system_clock::now();

    std::vector<std::string> codepad =
        {
            "789",
            "456",
            "123",
            ".0A"};

    std::vector<std::string> movepad =
        {
            ".^A",
            "<v>"};

    std::array<std::array<std::string, 256>, 256> keypadMap = generateMap(codepad);
    std::array<std::array<std::string, 256>, 256> movepadMap = generateMap(movepad);

    // generate permutations for joystick
    std::array<std::array<std::vector<std::string>, 256>, 256> movepadPermutations = generateStepPermutations(movepad, movepadMap);
    std::array<std::array<std::vector<std::string>, 256>, 256> codepadPermutations = generateStepPermutations(codepad, keypadMap);


    std::ifstream file("./input/21.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::vector<std::pair<int, int>> results;
    std::vector<std::string> codes;

    std::string input = buffer.str();
    int sum = 0;
    int next = 0, last = 0;

    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;

        codes.push_back(line);

        std::array<char, 2> padPositions = {'A', 'A'};
        std::vector<std::string> steps;

        char initialPos = 'A';
        std::vector<std::string> lowestSteps;
        int lowestStepsSize = -1;
        
        findLowestSteps(codepadPermutations, line, 'A', "", lowestSteps, lowestStepsSize, 0);
        
        //std::println("Code: {}", line);
        std::vector<std::string> allSteps;
        for (auto &st : lowestSteps)
            allSteps.push_back(st);

        // std::cin.get();
        steps = lowestSteps;
        int lowestFoundStepsSize = INT32_MAX;
        for (int i = 0; i < padPositions.size(); ++i)
        {
            lowestFoundStepsSize = INT32_MAX;
            steps = allSteps;
            allSteps.clear();
            
            //std::println("Len {}", steps.size());
            //printVector(steps);
            //std::cin.get();
            for (auto &step : steps)
            {
                lowestSteps = {};
                lowestStepsSize = -1;
                findLowestSteps(movepadPermutations, step, 'A', "", lowestSteps, lowestStepsSize, 0);
                //std::println("Num of Sol: {} Lowest Size {}", lowestSteps.size(), lowestStepsSize);
                if (lowestStepsSize <= lowestFoundStepsSize)
                {

                    lowestFoundStepsSize = lowestStepsSize;
                    for (auto &it : lowestSteps)
                    allSteps.push_back(it);
                }
            }
            std::erase_if(allSteps, [lowestFoundStepsSize](const std::string &elem)
                          { return elem.size() > lowestFoundStepsSize; });
        }
        //std::println("Final results:");
        //std::println("Code {} Best Sol {} Len {}", line, allSteps[0], allSteps[0].length());
        // std::println("Num of solutions: {} Len: {}, Steps: {}", steps.size(), steps[0].size(), steps);
        results.push_back({allSteps[0].length(), std::stoi(line.substr(0, 3))});
        //std::cin.get();
    }

    int i = 0;
    for (auto [len, num] : results)
    {
        int calc = len * num;
        std::println("{}: {} x {} = {}", codes[i++], len, num, calc);
        sum += calc;
    }

    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
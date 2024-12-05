#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <print>
#include <vector>
#include <algorithm>
#include <stack>

#define UNREACHABLE_SIGN 1000

bool sameSign(int a, int b)
{
    return (a ^ b) >= 0;
}

int main()
{
    std::ifstream file("./input/2.txt");
    std::stringstream buffer; 
    buffer << file.rdbuf();

    std::string input = buffer.str();

    int sum = 0;
    size_t last = 0;
    size_t next = 0;
    while ((next = input.find('\n', last)) != std::string::npos)
    {
        std::string line = input.substr(last, next-last);
        last = next + 1;


        // I don't like this solution but within scope of problem, it works :)
        // I'll improve later
        int sign = UNREACHABLE_SIGN; 
        int previousPlan = -1;
        std::stack<int> numStack;

        bool found = true;
        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] != ' ') 
                numStack.push(line[i] - int('0'));

            // keep looping until a whole number is added to stack
            if (!(i+1 == line.length() || line[i+1] == ' '))
                continue;

            // Calculate number from stack
            int plan = 0;
            int digit = 1;
            while (!numStack.empty())
            {
                plan += digit * numStack.top();
                digit *= 10;
                numStack.pop();
            }
            int diff = plan - previousPlan;

            
            if (previousPlan == -1)
            {
                previousPlan = plan;
                continue;
            }

            if (sign == UNREACHABLE_SIGN)
                sign = diff;

            if (diff == 0 || std::abs(diff) > 3 || !sameSign(diff, sign))
            {
                found = false;
                break;
            }

            previousPlan = plan;

        }

        sum += found ? 1 : 0;
    }

    std::print("{}\n", sum);

}
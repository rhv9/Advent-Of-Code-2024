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

int main()
{
    auto start = std::chrono::system_clock::now();
    
    std::ifstream file("./input/4.txt");
    std::stringstream buffer; 
    buffer << file.rdbuf();

    std::string input = buffer.str();

    std::vector<std::string> inputs;

    int sum = 0;
    size_t last = 0;
    size_t next = 0;
    while ((next = input.find("\n", last)) != std::string::npos)
    {
        std::string line = input.substr(last, next - last);
        last = next + 1;

        inputs.push_back(line);
    }


    const std::string xmas("XMAS");

    for (int i = 0; i < inputs.size(); i++)
    {
        for (int j = 0; j < inputs[i].length(); j++)
        {
            if (inputs[i][j] != 'X')
                continue;

            // check for XMAS in all directions.
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {   
                    bool matched = true;
                    for (int p = 0; p < xmas.size(); p++)
                    {   
                        int xpoint = j + x * (p);
                        int ypoint = i + y * (p);
                        if (ypoint < 0 || ypoint >= inputs.size() ||
                            xpoint < 0 || xpoint >= inputs[ypoint].length() ||
                            inputs[ypoint][xpoint] != xmas[p]) 
                        {
                            matched = false;
                            break;
                        }
                    }
                    if (matched)
                        ++sum;
                }
            }
        }
    }
    
    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("{}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);

}
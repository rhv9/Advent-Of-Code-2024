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

    std::ifstream file("./input/9.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();

    long sum = 0;

    int i = 0, reverse_i = input.size()-1;
    int reverse_count = input[reverse_i] - int('0');
    std::println("reverse {}", reverse_i);
    int count = 0;
    // 0/2 = 0, 2/2 = 1, 4/2 = 2
    while (i < reverse_i)
    {
        int num = input[i] - int('0');
        for (int j = 1; j < num+1; ++j)
        {

            sum += count * int(i/2);
            ++count;
            std::print("{}", int(i/2));
            std::cin.get();
        }

        // the gap
        ++i;
        num = input[i] - int('0');    

        for (int j = 0; j < num; ++j)
        {   
            if (reverse_count <= 0)
            {
                reverse_i -= 2;
                reverse_count = input[reverse_i] - int('0');
            }
            sum += count * int(reverse_i/2);
            ++count;
            --reverse_count;
            std::print("{}", int(reverse_i/2));
            std::cin.get();
        }// 
        ++i;
        //std::println("sum: {}", sum);
    }


    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
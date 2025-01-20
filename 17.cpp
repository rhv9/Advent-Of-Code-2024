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

constexpr uint8_t ADV = 0;
constexpr uint8_t BXL = 1;
constexpr uint8_t BST = 2;
constexpr uint8_t JNZ = 3;
constexpr uint8_t BXC = 4;
constexpr uint8_t OUT = 5;
constexpr uint8_t BDV = 6;
constexpr uint8_t CDV = 7;

#ifdef TEST
    #define PRINT(x, ...) std::println(x, __VA_ARGS__)
#else
    #define PRINT(x,...)
#endif

uint32_t zero = 0;
uint32_t one = 1;
uint32_t two = 2;
uint32_t three = 3;
// wrong solution: 616424735
uint32_t regA = 0, regB = 0, regC = 0;
std::vector<uint8_t> program;
int ir = 0;
uint32_t* comboOperand[8];

uint8_t getOperand(int instructionPointer)
{
    return program[instructionPointer+1];
}

std::optional<int> runADV()
{
    PRINT("Running ADV");
    uint32_t numerator = regA;
    uint32_t denominator = std::pow(2,*comboOperand[getOperand(ir)]);
    regA = numerator / denominator;

    return {};
}
std::optional<int> runBXL()
{
    PRINT("Running BXL");
    regB = regB ^ getOperand(ir);
    return {};
}
std::optional<int> runBST()
{
    PRINT("Running BST");
    regB = (*comboOperand[getOperand(ir)]) & 7;
    return {};
}
std::optional<int> runJNZ()
{
    PRINT("Running JNZ");
    if (regA)
        ir = getOperand(ir)-2;
    return {};
}
std::optional<int> runBXC()
{
    PRINT("Running BXC");
    regB = regB ^ regC;
    return {};
}
std::optional<int> runOUT()
{
    PRINT("Running OUT {}", (*comboOperand[getOperand(ir)]));
    return (*comboOperand[getOperand(ir)]) % 8;
}
std::optional<int> runBDV()
{
    PRINT("Running BDV");
    uint32_t numerator = regA;
    uint32_t denominator = std::pow(2,*comboOperand[getOperand(ir)]);
    regB = numerator / denominator;

    return {};
}
std::optional<int> runCDV()
{
    PRINT("Running CDV");
    uint32_t numerator = regA;
    uint32_t denominator = std::pow(2,*comboOperand[getOperand(ir)]);
    regC = numerator / denominator;

    return {};
}



int main()
{
    comboOperand[0] = &zero;
    comboOperand[1] = &one;
    comboOperand[2] = &two;
    comboOperand[3] = &three;
    comboOperand[4] = &regA;
    comboOperand[5] = &regB;
    comboOperand[6] = &regC;
    
    std::optional<int> (* executeOpcode[8])() = {};
    executeOpcode[ADV] = runADV;
    executeOpcode[BXL] = runBXL;
    executeOpcode[BST] = runBST;
    executeOpcode[JNZ] = runJNZ;
    executeOpcode[BXC] = runBXC;
    executeOpcode[OUT] = runOUT;
    executeOpcode[BDV] = runBDV;
    executeOpcode[CDV] = runCDV;    

    auto start = std::chrono::system_clock::now();

    std::ifstream file("./input/17.txt");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string input = buffer.str();

    int next = 0, last = 0;

    // generate map
    regA = std::stoi(input.substr(last + std::string("Register A: ").length(), (next = input.find("\n", last)) - last));
    last = next + 1;
    regB = std::stoi(input.substr(last + std::string("Register B: ").length(), (next = input.find("\n", last)) - last));
    last = next + 1;
    regC = std::stoi(input.substr(last + std::string("Register C: ").length(), (next = input.find("\n", last)) - last));
    last = next + 2;
    PRINT("Reg A: {}\nReg B: {}\nReg C: {}", regA, regB, regC);
    std::string programStr = input.substr(last + std::string("Program: ").length(),  (next = input.find("\n", last)) - last);

    for (int i = 0; i < programStr.length(); i += 2)
        program.push_back(programStr[i] - int('0'));

    //std::print("Program is ");
    //for (int i = 0; i < program.size(); ++i)
    //    std::print("{},", program[i]);
    //PRINT("");

    std::vector<uint8_t> outputs;
    //program = {0, 3};
    //regA = 16;

    while (ir < program.size())
    {
        auto output = executeOpcode[program[ir]]();
        ir+=2;

        //PRINT("Reg A: {}\nReg B: {}\nReg C: {}", regA, regB, regC);
        if (output.has_value())
            outputs.push_back(output.value());
    }

    // I have the power of C++, but I ALSO have the power of C
    char* outputString = new char[outputs.size()*2];
    outputString[outputs.size()*2-1] = '\0';
    outputString[0] = outputs[0] + '0';
    for (int i = 2; i < outputs.size()*2; i+=2)
    {
        outputString[i-1] = ',';
        outputString[i] = outputs[i/2] + '0';
    }

    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", outputString);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
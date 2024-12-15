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


class MathElement
{
public:
    virtual bool isOperand() = 0;
    virtual void print() = 0;
    virtual void println()
    {
        this->print();
        std::println();
    }

};

class NumElement : public MathElement
{
public:
    NumElement() : val(0) {}
    NumElement(uint64_t value) : val(value) {}
    virtual bool isOperand() override { return false; }

    virtual void print() override
    {
        std::print("{} ", val);
    }

    uint64_t val;
};

class OperatorElement : public MathElement
{
public:
    OperatorElement(char c) : op(c) {}
    virtual bool isOperand() override { return true; }

    uint64_t result(const uint64_t left, const uint64_t right)
    {
        switch (op)
        {
            case '+':
                return left + right;
                break;
            case '*':
                return left * right;
                break;
        }
        return 0;
    }

    uint64_t result(const NumElement& left, const NumElement& right)
    {
        return result(left.val, right.val);
    }


    virtual void print() override
    {
        std::print("{} ", op);
    }

    char op;
    
};

std::optional<uint64_t> getNumber(const std::string &input, int &pointer)
{
    int initialPointer = pointer;
    while (pointer >= 0 && pointer < input.size() &&
           input[pointer] >= int('0') && input[pointer] <= int('9'))
    {
        ++pointer;
    }

    std::string numString = input.substr(initialPointer, pointer);
    return !numString.length() ? std::optional<uint64_t>{} : std::stol(numString);
}

uint64_t solvePostFix(std::stack<MathElement*>& postfix)
{
    //std::stack<MathElement*> copy = postfix;
    //while (!copy.empty())
    //{
    //    copy.top()->print();
    //    copy.pop();
    //}
    //std::println();

    std::stack<uint64_t> numberStack;
    while (!postfix.empty())
    {
        if (postfix.top()->isOperand())
        {
            if (numberStack.size() < 2)
            {   
                std::println("ERROR, not enough numbers for operator!");
                exit(1);
            }

            uint64_t left = numberStack.top();
            numberStack.pop();
            uint64_t right = numberStack.top();
            numberStack.pop();
            OperatorElement* opElem = (OperatorElement*)postfix.top();
            
            uint64_t result = opElem->result(left, right);
            numberStack.push(result);
            postfix.pop();
        }
        else
        {
            numberStack.push(((NumElement*)postfix.top())->val);
            postfix.pop();
        }

    }

    return numberStack.top();
}

uint64_t calculate(const std::vector<uint64_t>& numbers, const std::vector<char>& operators)
{
    std::stack<MathElement*> postfix;
    std::stack<char> operatorStack;

    postfix.push(new NumElement(numbers[0]));
    for (int i = 0; i < operators.size(); i++)
    {
        char op = operators[i];

        if (operatorStack.size() == 0)
            operatorStack.push(op);
        else if (operatorStack.top() == '+' && op == '*')
            operatorStack.push(op);
        else
        {
            char prevOp = operatorStack.top();
            operatorStack.pop();
            operatorStack.push(op);
            postfix.push(new OperatorElement(prevOp));
        }

        postfix.push(new NumElement(numbers[i+1]));
    }

    // clear remaining operators
    while (!operatorStack.empty())
    {   
        postfix.push(new OperatorElement(operatorStack.top()));
        operatorStack.pop();
    }

    // reverse as it is in wrong order
    std::stack<MathElement*> reversePostfix;
    while (!postfix.empty())
    {
        reversePostfix.push(postfix.top());
        postfix.pop();
    }

    return solvePostFix(reversePostfix);
}

bool tryAllCombination(uint64_t target, const std::vector<uint64_t>& numbers, std::vector<char>& operators, int max)
{
    if (operators.size() >= max)
    {   
        //std::print("{}", numbers[0]);
        //for (int i = 0; i < operators.size(); i++)
        //{
        //    std::print(" {} {}", operators[i], numbers[i+1]);
        //}
        //std::println("");
        
        uint64_t result = calculate(numbers, operators);
        //std::println("Result = {}", result);
        //std::cin.get();
        return result == target;
    }
    operators.push_back('+');
    auto outcome = tryAllCombination(target, numbers, operators, max);
    if (outcome)
        return true;
    operators.pop_back();
    operators.push_back('*');
    outcome = tryAllCombination(target, numbers, operators, max);
    operators.pop_back();
    return outcome;
}

int main()
{
    auto start = std::chrono::system_clock::now();

    std::ifstream file("./input/7.txt");
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

        int pointer = 0;
        auto target = getNumber(line, pointer);
        pointer += 2;

        std::vector<uint64_t> numbers;
        numbers.reserve(12);
        
        while (pointer < line.length())
        {
            auto num = getNumber(line, pointer);
            if (num.has_value())
                numbers.push_back(num.value());
            ++pointer;
        }
        std::vector<char> operators;
        operators.reserve(line.size()-1);
        bool outcome = tryAllCombination(target.value(), numbers, operators, numbers.size()-1);

        if (outcome)
        {
            std::println("target: {} met", target.value());
            sum += target.value();
        }
    }


    auto elapsed = std::chrono::system_clock::now() - start;
    auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::println("Solution is {}", sum);
    std::println("Elapsed time {}ms", nanoSeconds / 1000000.0);
}
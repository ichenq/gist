// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.
//
// Reverse Polish notation
//
// Assume an input of a correct, space separated, string of tokens of an RPN expression
// eg. 3.59 4.99 2 * 1.79 5 - 2 3 ^ ^ / +
//

#include <assert.h>
#include <sstream>
#include <string>
#include <stack>
#include <iostream>

using namespace std;


void CalculateRPN(const std::string& expr)
{
    std::stack<double> stk;
    std::istringstream stream(expr);
    std::string token;
    while (stream >> token)
    {
        double number;
        if (std::istringstream(token) >> number) // is operand?
        {
            stk.push(number);
            cout << "push " << number << endl;
            continue;
        }
        cout << "operate " << token << endl;
        assert(stk.size() >= 2);
        double rightOprand = stk.top();
        stk.pop();
        double leftOprand = stk.top();
        stk.pop();
        if (token == "+")
        {
            stk.push(leftOprand + rightOprand);
        }
        else if (token == "-")
        {
            stk.push(leftOprand - rightOprand);
        }
        else if (token == "*")
        { 
            stk.push(leftOprand * rightOprand);
        }
        else if (token == "/")
        {
            stk.push(leftOprand / rightOprand);
        }
        else if (token == "%")
        {
            stk.push(fmod(leftOprand, rightOprand));
        }
        else if (token == "^")
        {
            stk.push(std::pow(leftOprand, rightOprand));
        }
        else
            assert(false);
    }
    assert(stk.size() == 1);
    cout << "result: " << stk.top() << endl;
}

int OperatorPrecedence(const std::string& op)
{
    if (op == "+" || op == "-")
        return 1;
    else if (op == "*" || op == "/")
        return 2;
    else if (op == "(" || op == ")")
        return 10;
    return -1;
}

bool HasHigherPrecedence(const std::string& op1, const std::string& op2)
{
    return OperatorPrecedence(op1) > OperatorPrecedence(op2);
}

// 当读到一个操作数的时候，立即把它放到输出中，操作符和左括号压入栈中。
// 如果读到一个右括号，则将栈元素弹出知道遇到一个对应的左括号；
// 如果读到操作符，那么从栈中弹出栈元素知道发现优先级更低的操作符为止；
// 最后，读到输入末尾，将栈元素弹出直到变为空栈。
std::string InfixToPostfix(const std::string& expr)
{
    std::string output;
    output.reserve(expr.size());
    std::istringstream input(expr);
    std::stack<std::string> stk;
    std::string token;
    while (input >> token)
    {
        double number;
        if (std::istringstream(token) >> number) // is operand?
        {
            output += token;
            output.push_back(' ');
            continue;
        }
        if (token == "(")
        {
            stk.push(token);
        }
        else if (token == ")")
        {
            while (!stk.empty() && stk.top() != "(")
            {
                output += stk.top();
                output.push_back(' ');
                stk.pop();
            }
            stk.pop();
        }
        else
        {
            while (!stk.empty() &&
                stk.top() != "(" &&
                HasHigherPrecedence(stk.top(), token))
            {
                output += stk.top();
                output.push_back(' ');
                stk.pop();
            }
            stk.push(token);
        }
    }
    while (!stk.empty())
    {
        output += stk.top();
        output.push_back(' ');
        stk.pop();
    }
    return output;
}
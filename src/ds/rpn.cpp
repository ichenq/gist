// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.
//
// Postfix(Reverse Polish notation) and infix conversion
//

#include <assert.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <stack>
#include <iostream>

using namespace std;

// Assume an input of a correct, space separated, string of tokens of an RPN expression
// eg. 3.59 4.99 2 * 1.79 5 - 2 3 ^ ^ / +
double CalculatePostfix(const std::string& expr)
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
    return stk.top();
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

// 1.	Scan the Infix string from left to right.
// 2.	Initialize an empty stack.
// 3.	If the scanned character is an operand, add it to the Postfix string.
// 4.	If the scanned character is an operator and if the stack is empty push the character to stack.
// 5.	If the scanned character is an Operator and the stack is not empty, compare the precedence of the 
//        character with the element on top of the stack.
// 6.	If top Stack has higher precedence over the scanned character pop the stack else push the scanned 
//        character to stack.Repeat this step until the stack is not empty and top Stack has precedence 
//        over the character.
// 7.	Repeat 4 and 5 steps till all the characters are scanned.
// 8.	After all characters are scanned, we have to add any character that the stack may have to the 
//        Postfix string.
// 9.	If stack is not empty add top Stack to Postfix string and Pop the stack.
// 10.	Repeat this step as long as stack is not empty.
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

// 1. While there are input symbol left
// 2.	Read the next symbol from input.
// 3.	If the symbol is an operand
//          Push it onto the stack.
// 4.	Otherwise,
//          the symbol is an operator.
// 5.	If there are fewer than 2 values on the stack
//          Show Error 
// 6.	Else
//          Pop the top 2 values from the stack.
//          Put the operator, with the values as arguments and form a string.
//          Encapsulate the resulted string with parenthesis.
//          Push the resulted string back to stack.
// 7.	If there is only one value in the stack
//          That value in the stack is the desired infix string.
// 8.	If there are more values in the stack
std::string PostfixToInfix(const std::string& expr)
{
    std::istringstream input(expr);
    std::stack<std::string> stk;
    std::string token;
    while (input >> token)
    {
        double number;
        if (std::istringstream(token) >> number) // is operand?
        {
            stk.push(token);
            continue;
        }
        assert(stk.size() >= 2);
        auto lhs = stk.top();
        stk.pop();
        auto rhs = stk.top();
        stk.pop();

        char buf[256];
        sprintf_s(buf, "( %s %s %s )", lhs.c_str(), token.c_str(), rhs.c_str());
        stk.push(buf);
    }
    assert(stk.size() == 1);
    return stk.top();
}
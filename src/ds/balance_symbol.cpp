// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.
//
// Check for balanced parentheses in an expression using stack

#include <string>
#include <stack>


inline bool ArePair(char opening, char closing)
{
    switch (opening)
    {
    case '(':
        return closing == ')';
    case '{':
        return closing == '}';
    case '[':
        return closing == ']';
    default:
        return false;
    }
}

bool AreParanthesesBalanced(const std::string& expr)
{
    std::stack<char> stk;
    for (auto ch : expr)
    {
        switch (ch)
        {
        case '(':
        case '{':
        case '[':
            stk.push(ch);
            break;
        case ')':
        case '}':
        case ']':
            if (stk.empty())
            {
                return false;
            }
            if (!ArePair(stk.top(), ch))
            {
                return false;
            }
            stk.pop();
            break;
        }
    }
    return stk.empty();
}
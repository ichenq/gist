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

// 做一个空栈，读入字符直至文件尾。如果字符是一个开放符号，则将其入栈。
// 如果字符是一个封闭符号，那么若栈为空，则报错；若栈不为空，则将栈元素弹出。
// 如果弹出的符号不是对应的开放符号，则报错。在文件尾，如果栈非空则报错。
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
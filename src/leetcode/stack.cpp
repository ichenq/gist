#include "stack.h"
#include <assert.h>
#include <stdint.h>
#include <cctype>
#include <stack>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>


using namespace std;

// https://leetcode.com/problems/min-stack/
class MinStack {
public:
    struct Node
    {
        int value;
        int min;

        Node(int v, int m)
            : value(v), min(m)
        {}
    };

    MinStack() {
    }

    void push(int x) {
        if (stack_.empty())
        {
            stack_.push(Node(x, x));
        }
        else
        {
            const Node& v = stack_.top();
            stack_.push(Node(x, std::min(x, v.min)));
        }
    }

    void pop() {
        stack_.pop();
    }

    int top() {
        const Node& v = stack_.top();
        return v.value;
    }

    int getMin() {
        const Node& v = stack_.top();
        return v.min;
    }

private:
    std::stack<Node>  stack_;
};

// https://leetcode.com/problems/implement-queue-using-stacks/
class MyQueue {
public:
    /** Initialize your data structure here. */
    MyQueue() {

    }

    /** Push element x to the back of queue. */
    void push(int x) {
        stack1_.push(x);
    }

    /** Removes the element from in front of queue and returns that element. */
    int pop() {
        while (!stack1_.empty()) {
            int v = stack1_.top();
            stack1_.pop();
            stack2_.push(v);
        }
        int n = stack2_.top();
        stack2_.pop();
        while (!stack2_.empty())
        {
            int v = stack2_.top();
            stack2_.pop();
            stack1_.push(v);
        }
        return n;
    }

    /** Get the front element. */
    int peek() {
        while (!stack1_.empty()) {
            int v = stack1_.top();
            stack1_.pop();
            stack2_.push(v);
        }
        int n = stack2_.top();
        while (!stack2_.empty())
        {
            int v = stack2_.top();
            stack2_.pop();
            stack1_.push(v);
        }
        return n;
    }

    /** Returns whether the queue is empty. */
    bool empty() {
        return stack1_.empty();
    }

private:
    std::stack<int> stack1_;
    std::stack<int> stack2_;
};

// https://leetcode.com/problems/backspace-string-compare/
bool backspaceCompare(const string& S, const string& T) {
    std::stack<char> stk1;
    std::stack<char> stk2;
    for (auto ch : S)
    {
        if (ch == '#') {
            if (!stk1.empty()) {
                stk1.pop();
            }
        }
        else {
            stk1.push(ch);
        }
    }
    for (auto ch : T)
    {
        if (ch == '#') {
            if (!stk2.empty()) {
                stk2.pop();
            }
        }
        else {
            stk2.push(ch);
        }
    }
    int n = stk1.size();
    if (n != stk2.size())
        return false;
    while (n--) {
        char ch = stk1.top();
        if (ch != stk2.top()) {
            return false;
        }
        stk1.pop();
        stk2.pop();
    }
    return true;
}

// https://leetcode.com/problems/valid-parentheses/
bool isValid(string s)
{
    if (s.empty())
        return true;
    stack<char> stk;
    for (auto ch : s)
    {
        if (ch == '(' || ch == '{' || ch == '[')
            stk.push(ch);
        else if (ch == ')' || ch == '}' || ch == ']') {
            if (stk.empty())
                return false;
            char left = stk.top();
            stk.pop();
            if (ch == ')' && (left != '('))
                return false;
            else if (ch == '}' && (left != '{'))
                return false;
            else if (ch == ']' && (left != '['))
                return false;
        }
    }
    return stk.empty();
}



static void compute(std::stack<char>& ops, std::stack<int64_t>& vals)
{
    char op = ops.top();
    ops.pop();
    int64_t n2 = vals.top();
    vals.pop();
    int64_t n1 = vals.top();
    vals.pop();
    switch (op) {
    case '+': n1 += n2; break;
    case '-': n1 -= n2; break;
    }
    vals.push(n1);
}

// https://leetcode.com/problems/basic-calculator/
int calculate(const string& s) {
    if (s.empty())
        return 0;
    int64_t result = 0;
    int64_t sign = 1;
    int64_t num = 0;
    std::stack<int64_t> stk;
    stk.push(sign);
    for (int i = 0; i < s.length(); i++) {
        char ch = s[i];
        //if (ch == ' ') {
        //    continue;
        //}
        if (ch >= '0' && ch <= '9') {
            num = num * 10 + (ch - '0');
        }
        else if (ch == '+' || ch == '-') {
            result += sign * num;
            sign = (ch == '+' ? 1 : -1);
            num = 0;
        }
        else if (ch == '(') {
            stk.push(result);
            stk.push(sign);
            sign = 1;
            result = 0;
        }
        else if (ch == ')') {
            result += sign * num;
            result *= stk.top();
            stk.pop();
            result += stk.top();
            stk.pop();
            num = 0;
        }
    }
    result += sign * num;
    return (int)result;
}

// https://leetcode.com/problems/baseball-game/
int calPoints(const vector<string>& ops) {
    std::stack<int> stack;
    int sum = 0;
    for (int i = 0; i < ops.size(); i++)
    {
        const string& v = ops[i];
        switch (v[0])
        {
        case '+': // sum of the last two round's points
        {
            int last = stack.top();
            stack.pop();
            int n = last + stack.top();
            stack.push(last);
            stack.push(n);
            sum += n;
        }
        break;

        case 'D': // doubled data of the last round's points
        {
            int last = stack.top();
            int n = last + last;
            stack.push(n);
            sum += n;
        }
        break;

        case 'C': // invalidate last round
            sum -= stack.top();
            stack.pop();
            break;

        default:
        {
            int n = atoi(v.c_str());
            stack.push(n);
            sum += n;
        }
        }
    }
    
    return sum;
}


// https://leetcode.com/problems/next-greater-element-i/
vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
    stack<int> stk;
    unordered_map<int, int> dict;
    for (auto n : nums2)
    {
        while (!stk.empty() && n > stk.top()) {
            dict[stk.top()] = n;
            stk.pop();
        }
        stk.push(n);
    }

    vector<int> result;
    result.reserve(nums1.size());
    for (auto n : nums1) {
        auto iter = dict.find(n);
        if (iter != dict.end()) {
            result.push_back(iter->second);
        }
        else {
            result.push_back(-1);
        }
    }
   
    return std::move(result);
}


void test_stack()
{
    {
        string s = "()";
        bool v = isValid(s);
        assert(v);
    }
    {
        MinStack minStack;
        minStack.push(512);
        minStack.push(-1024);
        minStack.push(-1024);
        minStack.pop();
        int v = minStack.getMin();
        assert(v == -1024);
        minStack.pop();
        v = minStack.getMin();
        assert(v == 512);
    }

    {
        int n = calculate(" 2-1 + 2 ");
        assert(n == 3);

        n = calculate("(1)");
        assert(n == 1);

        n = calculate("2-(5-6)");
        assert(n == 3);
    }

    {
        vector<string> args{ "5","2","C","D","+" };
        int sum = calPoints(args);
        assert(sum == 30);
    }

    {
        vector<int> nums1{ 4,1,2 };
        vector<int> nums2{ 1,3,4,2 };
        auto result = nextGreaterElement(nums1, nums2);
        vector<int> expected{ -1, 3, -1 };
        assert(result == expected);
    }
}
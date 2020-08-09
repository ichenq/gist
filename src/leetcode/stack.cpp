
#include <assert.h>
#include <stdint.h>
#include <stack>
#include <string>
#include <algorithm>

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

inline bool is_space(int ch) {
    return ch == ' ' || ch == '\t' || ch == '\n';
}

static int64_t my_atoi(const string& s, int& j) {
    int64_t n = 0;
    while (j < s.size() && is_space(s[j])) { // skip leading space
        j++;
    }
    // negative sign
    int sign = (s[j] == '-');
    if (sign || s[j] == '+')
        j++;

    while (j < s.size() && (s[j] >= '0' && s[j] <= '9')) {
        n = n * 10 + s[j] - '0';
        j++;
    }
    return sign ? -n : n;
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

static bool is_operator(int ch)
{
    return ch == '+' || ch == '-';
}

// https://leetcode.com/problems/basic-calculator/
int calculate(const string& s) {
    std::stack<char> ops;
    std::stack<int64_t> vals;
    int i = 0;
    while (i < s.size())
    {
        char ch = s[i];
        if (is_space(ch)) {
            i++;
            continue;
        }
        else if (ch == '(') {
            i++;
            continue;
        }
        else if (is_operator(ch)) {
            if (i == 0 || (i > 0 && is_operator(s[i-1]))) {
                int64_t n = my_atoi(s, i);
                vals.push(n);
                continue;
            }
            if (!ops.empty()) {
                compute(ops, vals);
            }
            ops.push(ch);
            i++;
            continue;
        }
        else if (ch == ')') {
            if (!ops.empty()) {
                compute(ops, vals);
            }
            i++;
            continue;
        } 
        else {
            int64_t n = my_atoi(s, i);
            vals.push(n);
        }
    }
    if (!ops.empty()) {
        compute(ops, vals);
    }
    assert(!vals.empty());
    int64_t n = vals.top();
    return (int)n;
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
        assert(n == -19);
    }
}
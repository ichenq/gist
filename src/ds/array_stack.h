// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <array>


struct Destroy 
{
    template<class T> 
    static void destroy(T& t) 
    { 
        t.~T(); 
    }
};

// Two stacks in an array
template <typename T, size_t N>
class ArrayStack
{
public:
    ArrayStack()
    {
    }

    ~ArrayStack()
    {
        while (TopSize() > 0)
        {
            PopTop();
        }
        while (BottomSize() > 0)
        {
            PopBottom();
        }
    }

    // upper stack
    const T& Top()
    {
        return array_[top_];
    }

    void PushTop(const T& value)
    {
        if (!IsFull())
        {
            array_[++top_] = value;
        }
    }

    void PopTop()
    {
        if (TopSize() > 0)
        {
            Destroy::destroy(array_[top_--]);
        }
    }

    size_t TopSize() const
    {
        return (top_ >= 0 ? top_ + 1 : 0);
    }

    // lower stack
    const T& Bottom()
    {
        return array_[bottom_]
    }

    void PushBottom(const T& value)
    {
        if (!IsFull())
        {
            array_[--bottom_] = value;
        }
    }

    void PopBottom()
    {
        if (BottomSize() > 0)
        {
            Destroy::destroy(array_[bottom_++]);
        }
    }

    size_t BottomSize() const
    {
        return N - bottom_;
    }

private:
    bool IsFull() const 
    {
        return top_ == bottom_;
    }

private:
    int top_ = -1;      // current top item
    int bottom_ = N;  // current bottom item

    std::array<T, N>    array_;
};

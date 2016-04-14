#pragma once

#include <assert.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
#include <ostream>


template <typename T>
class MinHeap
{
public:
    MinHeap()
    {
    }

    MinHeap(std::initializer_list<T> il)
        : array_(il)
    {
        std::make_heap(array_.begin(), array_.end(), pred_);
    }

    MinHeap(const std::vector<T>& v)
        : array_(v)
    {
        std::make_heap(array_.begin(), array_.end(), pred_);
    }

    MinHeap(std::vector<T>&& v)
        : array_(v)
    {
        std::make_heap(array_.begin(), array_.end(), pred_);
    }

    ~MinHeap()
    {
    }

    size_t size() const 
    {
        return array_.size();
    }

    void push(const T& item)
    {
        array_.push_back(item);
        std::push_heap(array_.begin(), array_.end(), pred_);
    }

    void push(T&& item)
    {
        array_.emplace_back(item);
        std::push_heap(array_.begin(), array_.end(), pred_);
    }

    const T& root() const
    {
        assert(!array_.empty());
        return array_[0];
    }

    T root()
    {
        assert(!array_.empty());
        return array_[0];
    }

    void pop()
    {
        assert(!array_.empty());
        std::pop_heap(array_.begin(), array_.end(), pred_);
        array_.pop_back();
    }

    void print(std::ostream& out, const char* dim = " ")
    {
        std::ostream_iterator<T> iter(out, dim);
        std::copy(array_.begin(), array_.end(), iter);
        out << std::endl;
    }

private:
    std::vector<T>      array_;
    std::greater<T>     pred_;
};
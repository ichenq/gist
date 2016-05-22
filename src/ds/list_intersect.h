#pragma once

#include <list>

// both `l1` and `l2` are ordered
template <typename T>
std::list<T> ListIntersection(const std::list<T>& l1, const std::list<T>& l2)
{
    std::list<T> intersect;
    auto iter1 = l1.begin();
    auto iter2 = l2.begin();
    while (iter1 != l1.end() && iter2 != l2.end())
    {
        if (*iter1 == *iter2)
        {
            intersect.push_back(*iter1);
            ++iter1;
            ++iter2;
        }
        else if (*iter1 < *iter2)
        {
            ++iter1;
        }
        else
        {
            ++iter2;
        }
    }
    return intersect;
}

// both `l1` and `l2` are ordered
template <typename T>
std::list<T> ListUnion(const std::list<T>& l1, const std::list<T>& l2)
{
    std::list<T> result;
    auto iter1 = l1.begin();
    auto iter2 = l2.begin();
    while (iter1 != l1.end() && iter2 != l2.end())
    {
        if (*iter1 == *iter2)
        {
            result.push_back(*iter1);
            ++iter1;
            ++iter2;
        }
        else if (*iter1 < *iter2)
        {
            result.push_back(*iter1++);
        }
        else
        {
            result.push_back(*iter2++);
        }
    }
    return result;
}

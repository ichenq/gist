// Copyright (C) 2013-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.
//
// Created at Nov 17, 2012
//
// 乘法 http://en.wikipedia.org/wiki/Multiplication_algorithm
// 除法 http://en.wikipedia.org/wiki/Division_algorithm
//
// 参考实现： 
//    GNU GMP库 http://gmplib.org/manual/Algorithms.html#Algorithms
//    BigInt库  http://mattmccutchen.net/bigint


#pragma once

#include <vector>
#include <iostream>

typedef int    Blk;
typedef std::vector<Blk>    NumberArray;


class BigNumber
{
public:
    enum {BASE = 10};
    enum {lessThan = -1, equalTo = 0, biggerThan = 1};

    BigNumber(void);
    explicit BigNumber(Blk val);
    explicit BigNumber(const std::string& number);
    ~BigNumber(void);

    bool    is_zero() const {return blk.empty();}

    int     compare(const BigNumber& b) const;

    BigNumber&  add(const BigNumber& b, BigNumber& c) const;   // c = a - b
    BigNumber&  sub(const BigNumber& b, BigNumber& c) const;   // c = a + b
    BigNumber&  mul(const BigNumber& b, BigNumber& c) const;   // c = a * b
    BigNumber&  div(const BigNumber& b, BigNumber& q, BigNumber& r) const;   // a = b * q + r
    BigNumber&  lshift(Blk bits, BigNumber& c) const;     // c = a << bits

    std::string  to_string();

private:
    void    init_from(Blk val);
    void    init_from(const std::string& number);

    void    mul_to(Blk multiplier, size_t start_pos, NumberArray& blk) const;
    BigNumber&    trim();

    NumberArray    blk;
};

inline BigNumber operator + (const BigNumber& a, const BigNumber& b)
{
    BigNumber c;
    a.add(b, c);
    return c;
}

inline BigNumber operator - (const BigNumber& a, const BigNumber& b)
{
    BigNumber c;
    a.sub(b, c);
    return c;
}

inline BigNumber operator * (const BigNumber& a, const BigNumber& b)
{
    BigNumber c;
    a.mul(b, c);
    return c;
}

inline BigNumber operator / (const BigNumber& a, const BigNumber& b)
{
    BigNumber q, r;
    a.div(b, q, r);
    return q;
}

inline BigNumber operator % (const BigNumber& a, const BigNumber& b)
{
    BigNumber q, r;
    a.div(b, q, r);
    return r;
}

inline bool operator < (const BigNumber& a, const BigNumber& b)
{
    return a.compare(b) < 0;
}

inline bool operator > (const BigNumber& a, const BigNumber& b)
{
    return (b < a);
}

inline bool operator == (const BigNumber& a, const BigNumber& b)
{
    return a.compare(b) == 0;
}

inline bool operator != (const BigNumber& a, const BigNumber& b)
{
    return !(a == b);
}

inline bool operator <= (const BigNumber& a, const BigNumber& b)
{
    return !(a > b);
}

inline bool operator >= (const BigNumber& a, const BigNumber& b)
{
    return !(a < b);
}


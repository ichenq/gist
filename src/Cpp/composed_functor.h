// Copyright (C) 2010-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#ifndef COMPOSE_FUNCTOR_H
#define COMPOSE_FUNCTOR_H

#include <functional>

//
//compose function adapter, f(g(x)), f(g(x, y)), f(g(x), h(x)), f(g(x), h(y))
//


// compose_f_gx adapter, which has f(g(x)) semantics.
template <typename OP1, typename OP2>
class compose_f_gx_t
    : std::unary_function<typename OP2::argument_type, typename OP1::result_type>
{
public:
    compose_f_gx_t(const OP1& f, const OP2& h)
        : f_(f), g_(h)
    {
    }

    typename OP1::result_type
    operator () (const typename OP2::argument_type& x) const
    {
        return f_(g_(x));
    }

private:
    OP1     f_;
    OP2     g_;
};

template <typename OP1, typename OP2>
inline compose_f_gx_t<OP1, OP2>
compose_f_gx(const OP1& op1, const OP2& op2)
{
    return compose_f_gx_t<OP1, OP2>(op1, op2);
}



//  compose_f_gxy adapter, which has f(g(x, y)) semantics.
template <typename OP1, typename OP2>
class compose_f_gxy_t
    : std::binary_function < typename OP2::argument_type, typename OP2::argument_type,
      typename OP1::result_type >
{
public:
    compose_f_gxy_t(const OP1& f, const OP2& g)
        : f_(f), g_(g)
    {
    }

    typename OP1::result_type
    operator () (const typename OP2::argument_type& x, const typename OP2::argument_type& y) const
    {
        return f_(g_(x), g_(y));
    }

private:
    OP1     f_;
    OP2     g_;
};

template <typename OP1, typename OP2>
inline compose_f_gxy_t<OP1, OP2>
compose_f_gxy(const OP1& f, const OP2& g)
{
    return compose_f_gxy_t<OP1, OP2>(f, g);
}


//  compose_f_gx_hx adapter, which has f(g(x), h(x)) semantics.
template <typename OP1, typename OP2, typename OP3>
class compose_f_gx_hx_t
    : std::unary_function<typename OP2::argument_type, typename OP1::result_type>
{
public:
    compose_f_gx_hx_t(const OP1& f, const OP2& g, const OP3& h)
        : f_(f), g_(g), h_(h)
    {
    }

    typename OP1::result_type
    operator () (const typename OP2::argument_type& x) const
    {
        return f_(g_(x), h_(x));
    }

private:
    OP1     f_;
    OP2     g_;
    OP3     h_;
};


template <typename OP1, typename OP2, typename OP3>
inline compose_f_gx_hx_t<OP1, OP2, OP3>
compose_f_gx_hx(const OP1& f, const OP2& g, const OP3& h)
{
    return compose_f_gx_hx_t<OP1, OP2, OP3>(f, g, h);
}


//  compose_f_gx_hy adapter, which has f(g(x), h(y)) semantics.
template <typename OP1, typename OP2, typename OP3>
class compose_f_gx_hy_t
    : std::binary_function < typename OP2::argument_type, typename OP3::argument_type,
      typename OP1::result_type >
{
public:
    compose_f_gx_hy_t(const OP1& f, const OP2& g, const OP3& h)
        : f_(f), g_(g), h_(h)
    {
    }

    typename OP1::result_type
    operator () (const typename OP2::argument_type& x, const typename OP3::argument_type& y) const
    {
        return f_(g_(x), h_(y));
    }

private:
    OP1     f_;
    OP2     g_;
    OP3     h_;
};

template <typename OP1, typename OP2, typename OP3>
inline compose_f_gx_hy_t<OP1, OP2, OP3>
compose_f_gx_hy(const OP1& f, const OP2& g, const OP3& h)
{
    return compose_f_gx_hy_t<OP1, OP2, OP3>(f, g, h);
}


#endif // COMPOSE_FUNCTOR_H
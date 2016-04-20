// Copyright (C) 2015 chenqiang@chaoyuehudong.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include "stdafx.h"
#include "dynamic.h"
#include <functional>
#include <msgpack.hpp>

dynamic::TypeError::TypeError(const std::string& expected, dynamic::Type t)
    : std::runtime_error(to<std::string>("expected '", expected, 
        "' but get type '", dynamic::typeName(t), "'"))
{
}

struct dynamic::ObjectMaker
{
    friend struct dynamic;
};

//////////////////////////////////////////////////////////////////////////

dynamic::dynamic(std::nullptr_t)
    : type_(NULLT)
{
    u_.nul = nullptr;
}

dynamic::dynamic(bool value)
    : type_(BOOL)
{
    u_.boolean = value;
}

dynamic::dynamic(int64_t value)
    : type_(INT64)
{
    u_.integer = value;
}

dynamic::dynamic(double value)
    : type_(DOUBLE)
{
    u_.number = value;
}

dynamic::dynamic(const char* s, int count)
    : type_(STRING)
{
    assert(s);
    count = (count < 0 ? strlen(s) : count);
    u_.string = new std::string(s, count);
}

dynamic::dynamic(std::string&& s)
    : type_(STRING)
{
    u_.string = new std::string(std::move(s));
}

dynamic::dynamic(std::initializer_list<dynamic> il)
    : type_(ARRAY)
{
    u_.array = new Array(il.begin(), il.end());
}

dynamic::dynamic(ObjectMaker(*)())
    : type_(OBJECT)
{
    u_.object = new Object;
}

dynamic::ObjectMaker dynamic::object()
{
    return ObjectMaker();
}

dynamic& dynamic::operator=(const dynamic& o)
{
    if (&o != this)
    {
        assign(o);
    }
    return *this;
}

dynamic& dynamic::operator=(dynamic&& o) noexcept
{
    if (&o != this)
    {
        destroy();
        u_ = o.u_;
        type_ = o.type_;
        o.type_ = NULLT;
        o.u_.nul = nullptr;
    }
    return *this;
}

const std::string&  dynamic::getString() const
{
    if (isString())
    {
        return *u_.string;
    }
    throw TypeError("string", type());
}

double dynamic::getDouble() const
{
    if (isDouble())
    {
        return u_.number;
    }
    throw TypeError("double", type());
}

int64_t dynamic::getInt() const
{
    if (isInt())
    {
        return u_.integer;
    }
    throw TypeError("int64", type());
}

bool dynamic::getBool() const
{
    if (isBool())
    {
        return u_.boolean;
    }
    throw TypeError("bool", type());
}

std::size_t dynamic::size() const
{
    switch (type())
    {
    case STRING:
        return u_.string->size();
    case ARRAY:
        return u_.array->size();
    case OBJECT:
        return u_.object->size();
    }
    throw TypeError("string/array/object", type());
}

dynamic::const_iterator dynamic::begin() const
{
    if (isObject())
    {
        return u_.object->begin();
    }
    throw TypeError("object", type());
}

dynamic::const_iterator dynamic::end() const
{
    if (isObject())
    {
        return u_.object->end();
    }
    throw TypeError("object", type());
}

void dynamic::push_back(const dynamic& o)
{
    if (isArray())
    {
        u_.array->push_back(o);
    }
    else
    {
        throw TypeError("array", type());
    }
}

void dynamic::push_back(dynamic&& o)
{
    if (isArray())
    {
        u_.array->push_back(std::move(o));
    }
    else
    {
        throw TypeError("array", type());
    }
}

const dynamic& dynamic::at(size_t index) const
{
    if (isArray())
    {
        return u_.array->at(index);
    }
    throw TypeError("array", type());
}

dynamic& dynamic::at(size_t index)
{
    return const_cast<dynamic&>(const_cast<dynamic const*>(this)->at(index));
}

bool dynamic::contains(const std::string& key) const
{
    switch (type())
    {
    case OBJECT:
        {
            dynamic d(key);
            return u_.object->count(d) > 0;
        }
        case ARRAY:
            return false; // cuz Lua encode empty table as array
        default:
            throw TypeError("object", type());
    }
}

dynamic& dynamic::operator[](const std::string& key)
{
    if (isObject())
    {
        dynamic d(key);
        return u_.object->operator[](d);
    }
    throw TypeError("object", type());
}

const dynamic& dynamic::operator[](const std::string& key) const
{
    if (isObject())
    {
        dynamic d(key);
        auto iter = u_.object->find(d);
        if (iter != u_.object->end())
        {
            return iter->second;
        }
        throw std::out_of_range(to<std::string>("couldn't find key ", key));
    }
    throw TypeError("object", type());
}

void dynamic::assign(const dynamic& o)
{
    destroy();
    type_ = o.type();
    switch (o.type())
    {
    case BOOL:
        u_.boolean = o.u_.boolean;
        break;
    case INT64:
        u_.integer = o.u_.integer;
        break;
    case DOUBLE:
        u_.number = o.u_.number;
        break;
    case STRING:
        u_.string = new std::string(*o.u_.string);
        break;
    case ARRAY:
        u_.array = new Array(*o.u_.array);
        break;
    case OBJECT:
        u_.object = new Object(*o.u_.object);
        break;
    }
}

void dynamic::destroy()
{
    switch (type())
    {
    case STRING:
        delete u_.string;
        break;
    case ARRAY:
        delete u_.array;
        break;
    case OBJECT:
        delete u_.object;
        break;
    }
    u_.nul = nullptr;
}

std::string dynamic::dump() const
{
    std::ostringstream stream;
    stream << *this;
    return stream.str();
}

bool dynamic::operator<(const dynamic& o) const
{
    if (type() != o.type())
    {
        return type() < o.type();
    }
    switch (type())
    {
    case NULLT:
        return u_.nul < o.u_.nul;
    case BOOL:
        return u_.boolean < o.u_.boolean;
    case INT64:
        return u_.integer < o.u_.integer;
    case DOUBLE:
        return u_.number < o.u_.number;
    case STRING:
        return getString() < o.getString();
    case ARRAY:
        return *u_.array < *o.u_.array;
    case OBJECT:
        return *u_.object < *o.u_.object;
    }
    return false;
}

const void* dynamic::getAddress() const
{
    switch (type())
    {
    case NULLT:
        return &u_.nul;
    case BOOL:
        return &u_.boolean;
    case INT64:
        return &u_.integer;
    case DOUBLE:
        return &u_.number;
    case STRING:
        return &u_.string;
    case ARRAY:
        return u_.array;
    case OBJECT:
        return u_.object;
    }
    throw TypeError("any", type());
}

const char* dynamic::typeName(Type t)
{
    switch (t)
    {
    case NULLT: return "null";
    case ARRAY: return "array";
    case BOOL: return "bool";
    case DOUBLE: return "double";
    case INT64: return "integer";
    case OBJECT: return "object";
    case STRING: return "string";
    default: return "unknown type";
    }
}

std::ostream& operator<<(std::ostream& strm, const dynamic& d)
{
    switch (d.type())
    {
    case dynamic::NULLT:
        strm << "null";
        break;
    case dynamic::BOOL:
        strm << (d.getBool() ? "true" : "false");
        break;
    case dynamic::INT64:
        strm << d.getInt();
        break;
    case dynamic::DOUBLE:
        strm << d.getDouble();
        break;
    case dynamic::STRING:
        strm << "\"" << d.getString() << "\"";
        break;
    case dynamic::ARRAY:
        {
            strm << "[";
            for (auto i = 0U; i < d.size(); i++)
            {
                strm << d.at(i);
                if (i + 1 < d.size())
                    strm << ",";
            }
            strm << "]";
        }
        break;
    case dynamic::OBJECT:
        {
            strm << "{";
            for (auto iter = d.begin(); iter != d.end(); ++iter)
            {
                strm << iter->first << ":" << iter->second;
                auto next = iter;
                if (++next != d.end())
                    strm << ",";
            }
            strm << "}";
        }
        break;
    }
    return strm;
}

inline dynamic parseArray(const msgpack::object& obj)
{
    auto size = obj.via.array.size;
    auto ptr = obj.via.array.ptr;
    assert(ptr);
    dynamic vec = std::initializer_list<dynamic>();
    for (auto i = 0U; i < size; ++i)
    {
        vec.push_back(parseToDynamic(ptr[i]));
    }
    return std::move(vec);
}

inline dynamic parseObject(const msgpack::object& obj)
{
    auto size = obj.via.map.size;
    auto ptr = obj.via.map.ptr;
    dynamic object = dynamic::object;
    for (auto i = 0U; i < size; ++i)
    {
        auto item = ptr[i];
        auto k = parseToDynamic(item.key);
        auto v = parseToDynamic(item.val);
        const auto& s = k.as<std::string>();
        object[s] = v;
    }
    return std::move(object);
}

DynamicValue parseToDynamic(const msgpack::object& obj)
{
    switch (obj.type)
    {
    case msgpack::type::NIL:
        return dynamic(nullptr);

    case msgpack::type::BOOLEAN:
        return dynamic(obj.via.boolean);

    case msgpack::type::POSITIVE_INTEGER:
        return dynamic(obj.via.u64);

    case msgpack::type::NEGATIVE_INTEGER:
        return dynamic(obj.via.i64);

    case msgpack::type::FLOAT:
        return dynamic(obj.via.f64);

        // UTF-8 and binary string
    case msgpack::type::STR:
    case msgpack::type::BIN:
        {
            auto str = obj.via.str;
            return dynamic(str.ptr, str.size);
        }

        // array type
    case msgpack::type::ARRAY:
        return parseArray(obj);

        // object type
    case msgpack::type::MAP:
        return parseObject(obj);

    default:
        CHECK(0);
    }
    return dynamic(nullptr);
}
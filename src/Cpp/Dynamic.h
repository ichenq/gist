// Copyright (C) 2015 chenqiang@chaoyuehudong.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#pragma once

#include <stdint.h>
#include <map>
#include <vector>
#include <string>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <initializer_list>
#include <msgpack/object_fwd.hpp>
#include "Conv.h"


/**
 * This is a runtime dynamically typed value.  It holds types from a
 * specific predetermined set of types (ints, bools, arrays, etc).  In
 * particular, it can be used as a convenient in-memory representation
 * for complete json objects.
 *
 * In general you can try to use these objects as if they were the
 * type they represent (although in some cases with a slightly less
 * complete interface than the raw type), and it'll just throw a
 * TypeError if it is used in an illegal way.
 *
 * Some examples:
 *
 *   dynamic twelve = 12;
 *   dynamic str = "string";
 *   dynamic array = {twelve, 3.14159, "text"}
 *   dynamic map = dynamic::object;
 *   map[str] = array;
 *   
 */
struct dynamic
{
public:
    enum Type
    {
        NULLT,
        ARRAY,
        BOOL,
        DOUBLE,
        INT64,
        OBJECT,
        STRING,
    };

    struct ObjectMaker;

    struct TypeError : std::runtime_error
    {
        explicit TypeError(const std::string& expected, dynamic::Type actual);
    };

    typedef std::vector<dynamic> Array;
    typedef std::map<dynamic, dynamic>  Object;

public:
    dynamic(const dynamic& o) : type_(NULLT) { *this = o; }
    dynamic(dynamic&& o) noexcept : type_(NULLT) { *this = std::move(o); }
    ~dynamic() noexcept { destroy(); }

    /*
     * Conversion constructors from primitive types.
     */
    explicit  dynamic(std::nullptr_t = nullptr);
    explicit  dynamic(bool value);
    explicit  dynamic(int64_t value);

    /*
     * Delegating constructors
     */
    explicit dynamic(uint64_t value) : dynamic(int64_t(value)) {}
    explicit dynamic(uint32_t value) : dynamic(int64_t(value)) {}
    explicit dynamic(int32_t value) : dynamic(int64_t(value)) {}
    explicit dynamic(uint16_t value) : dynamic(int64_t(value)) {}
    explicit dynamic(int16_t value) : dynamic(int64_t(value)) {}
    explicit dynamic(uint8_t value) : dynamic(int64_t(value)) {}
    explicit dynamic(int8_t value) : dynamic(int64_t(value)) {}

    explicit dynamic(double value);
    explicit dynamic(float value) : dynamic(double(value)) {}

    /*
     * String compatibility constructors.
     */
    explicit dynamic(const char* s, int count);
    explicit dynamic(std::string&& s);
    explicit dynamic(const std::string& s) : dynamic(s.c_str(), s.size()) {}

    /*
     * Create a new array from an initializer list.
     *
     * For example:
     *
     *   dynamic v = { 1, 2, 3, "foo" };
     */
    /* implicit */ dynamic(std::initializer_list<dynamic> il);

    /*
     * For making dynamic objects.
     */
    static ObjectMaker object();

    /*
     * This is part of the plumbing for object(), above.  Used to create
     * a new object dynamic.
     */
    /* implicit */ dynamic(ObjectMaker(*)());
    /* implicit */ dynamic(ObjectMaker const&) = delete;
    /* implicit */ dynamic(ObjectMaker&&);

    /*
     * Returns the type of this dynamic.
     */
    Type type() const { return type_; }

    /*
     * Returns true if this dynamic is of the specified type.
     */
    bool isString() const { return type() == STRING; }
    bool isObject() const { return type() == OBJECT; }
    bool isBool() const { return type() == BOOL; }
    bool isNull() const { return type() == NULLT; }
    bool isArray() const { return type() == ARRAY; }
    bool isDouble() const { return type() == DOUBLE; }
    bool isInt() const { return type() == INT64; }
    bool isNumber() const { return isInt() || isDouble(); }

    /*
     * Extract a value while trying to convert to the specified type.
     * Throws exceptions if we cannot convert from the real type to the
     * requested type.
     *
     * Note you can only use this to access integral types or strings,
     * since arrays and objects are generally best dealt with as a
     * dynamic.
     */
    const std::string&  getString() const;
    double getDouble() const;
    int64_t getInt() const;
    bool getBool() const;

    template <typename Tgt>
    typename std::enable_if<std::is_same<Tgt, bool>::value,
        Tgt>::type as() const
    {
        return getBool();
    }

    template <typename Tgt>
    typename std::enable_if <
        std::is_integral<Tgt>::value && !std::is_same<Tgt, bool>::value,
        Tgt > ::type as() const
    {
        switch (type())
        {
        case INT64:
            return static_cast<Tgt>(getInt());
        case DOUBLE:
            return static_cast<Tgt>(getDouble());
        case STRING:
            return to<Tgt>(getString());
        default:
            throw TypeError("int/double/string", type());
        }
    }

    template <typename Tgt>
    typename std::enable_if<std::is_floating_point<Tgt>::value,
        Tgt>::type as() const
    {
        switch (type())
        {
        case INT64:
            return static_cast<Tgt>(getInt());
        case DOUBLE:
            return static_cast<Tgt>(getDouble());
        default:
            throw TypeError("int/double", type());
        }
    }

    template <typename Tgt>
    typename std::enable_if<std::is_same<Tgt, std::string>::value,
        Tgt>::type as() const
    {
        switch (type())
        {
        case STRING:
            return getString();
        case INT64:
            return to<std::string>(getInt());
        case DOUBLE:
            return to<std::string>(getDouble());
        case BOOL:
            return (getBool() ? "true" : "false");
        case NULLT:
            return ("null");
        default:
            throw TypeError("int/double/string", type());
        }
    }

    template <typename T>
    operator T() const
    {
        return as<T>();
    }

    /*
     * If this is an array or an object, returns the number of elements
     * contained.  If it is a string, returns the length.  Otherwise
     * throws TypeError.
     */
    std::size_t size() const;

    bool empty() const { return size() == 0; }

    /*
     * You can iterate over the values of the object.  Calling these on
     * non-object will throw a TypeError.
     */
    typedef Object::const_iterator const_iterator;
    const_iterator begin()  const;
    const_iterator end()    const;

    /*
     * If this is an object, returns whether it contains a field with
     * the given name.  Otherwise throws TypeError.
     */
    bool contains(const std::string& key) const;

    /*
     * Assignment from other dynamics.  Because of the implicit conversion
     * to dynamic from its potential types, you can use this to change the
     * type pretty intuitively.
     *
     * Basic guarantee only.
     */
    dynamic& operator=(dynamic const&);
    dynamic& operator=(dynamic&&)noexcept;

    //bool operator==(const dynamic& o) const;
    //bool operator!=(const dynamic& o) const { return !(*this == o); }
    bool operator<(const dynamic& o) const;
    bool operator>(const dynamic& o) const { return !(o < *this); }

    /*
     * Append elements to an array.  If this is not an array, throws
     * TypeError.
     *
     * Invalidates iterators.
     */
    void push_back(dynamic const&);
    void push_back(dynamic&&);

    /*
     * For an object, the non-const overload inserts a null value
     * if the key isn't present.  The const overload will throw
     * std::out_of_range if the key is not present.
     *
     * These functions do not invalidate iterators.
     */
    dynamic&        operator[](const std::string&);
    const dynamic&  operator[](const std::string&)const;
    const dynamic&  operator[](const char* s) const
    {
        return this->operator[](std::string(s));
    }

    /*
     * For arrays, provides access to sub-fields by index.
     *
     * Using these with dynamic objects that are not arrays will
     * throw a TypeError.  Using an index that is out of range or
     * object-element that's not present throws std::out_of_range.
     */
    dynamic const& at(size_t index) const;
    dynamic&       at(size_t index);

    /*
     * For simple dynamics (not arrays or objects), this prints the
     * value to an std::ostream in the expected way.  Respects the
     * formatting manipulators that have been sent to the stream
     * already.
     *
     * If the dynamic holds an object or array, this prints them in a
     * format very similar to JSON.  (It will in fact actually be JSON
     * as long as the dynamic validly represents a JSON object---i.e. it
     * can't have non-string keys.)
     */
    friend std::ostream& operator<<(std::ostream&, const dynamic&);

    std::string dump() const;

    static const char* typeName(Type t);

private:
    void assign(const dynamic& o);
    void destroy();
    const void* getAddress() const;
    void* getAddress()
    {
        return const_cast<void*>(const_cast<const dynamic*>(this)->getAddress());
    }

private:
    Type type_;
    union Data
    {
        void*           nul;
        bool            boolean;
        int64_t         integer;
        double          number;
        std::string*    string;
        Array*          array;
        Object*         object;
    }u_;
};

// if object `d` contains field `key`, let v = d[key]
template <typename T>
inline T& SetObjectField(T& v, const dynamic& d, const std::string& key)
{
    if (d.contains(key))
    {
        v = d[key].as<T>();
    }
    return v;
}

typedef dynamic DynamicValue;

// parse msgpack object to dynamic value
DynamicValue parseToDynamic(const msgpack::object& obj);

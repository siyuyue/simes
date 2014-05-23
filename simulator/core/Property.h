// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#pragma once

#include <boost/function.hpp>
#include <sstream>
#include <string>

using namespace std;

typedef boost::function<bool (const string& s)> SetFunction;
typedef boost::function<bool (string& s)> GetFunction;

// Common interface to get/set a property of a component.
class CProperty {
private:
    string _key;
    string _description;
    SetFunction _setter;
    SetFunction _initialSetter;
    GetFunction _getter;
public:
    CProperty(string key, string description, GetFunction getter);
    CProperty(string key, string description, SetFunction setter, GetFunction getter);
    CProperty(string key, string description, SetFunction setter, GetFunction getter, SetFunction initialSetter);
    ~CProperty();
    bool Set(const string &value) const;
    bool SetInitial(const string &value) const;
    bool Get(string &value) const;
    string GetDescription() const;
    string GetKey() const;
};

namespace {
    // Null setter and getter functions
    bool NullSetter(const string& s) {
        return true;
    }

    bool NullGetter(string& s) {
        return true;
    }

    // Invalid setter and getter functions
    bool InvalidSetter(const string& s) {
        return false;
    }

    bool InvalidGetter(string& s) {
        return false;
    }

    // Conversion from string to int/float/double
    template <typename T>
    T FromString(const string& str) {
        T t;
        istringstream ss(str);
        ss >> t;
        return t;
    }

    // Conversion from int/float/double to string
    template <typename T>
    string ToString(const T t) {
        ostringstream ss;
        ss << t;
        return ss.str();
    }

    // Simple setter and getter functions
    template <typename T>
    bool SimpleSetter(const string& s, T& p) {
        p = FromString<T>(s);
        return true;
    }

    template <typename T>
    bool SimpleGetter(string& s, const T& p) {
        s = ToString<T>(p);
        return true;
    }

    // Check first and then set function
    template <typename T>
    bool CheckSetter(const string& s, T& p, boost::function<bool (const T&)> checker) {
        T pTemp = FromString<T>(s);
        if (!checker(pTemp)) {
            return false;
        }
        p = pTemp;
        return true;
    }
}

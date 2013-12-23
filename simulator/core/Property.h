// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#pragma once

#include <string>

using namespace std;

class CProperty {
private:
    string _key;
    string _defaultValue;
    Function _setter;
    Function _getter;
public:
    CProperty(string key, string defaultValue);
    ~CProperty();
    bool Set(string &value);
    bool Get(string &value) const;
    bool ResetDefault();
};
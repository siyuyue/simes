// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CSimException class
// **********************************************
#pragma once

#include <string>

using namespace std;

class CSimException : public exception {
public:
    CSimException(const char *name, const char *message);
    ~CSimException() throw();
    virtual const char * what() const throw();
    string componentName;
    string exceptionMessage;
};

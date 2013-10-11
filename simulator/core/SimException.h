// **********************************************
// CSimException class
// **********************************************
#pragma once

#include <string>

using namespace std;

class CSimException : public exception
{
public:
    CSimException(const char *name, const char *message);
    ~CSimException() throw();
    virtual const char * what() const throw();
    string componentName;
    string exceptionMessage;
};

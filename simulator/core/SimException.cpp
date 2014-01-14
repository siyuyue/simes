// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "core/SimException.h"

CSimException::CSimException(const char *name, const char *message) : componentName(name), exceptionMessage(message) {
}

CSimException::~CSimException() throw() {
}

const char * CSimException::what() const throw() {
    return exceptionMessage.c_str();
}

#include "SimException.h"

CSimException::CSimException(const char *name, const char *message) : componentName(name), exceptionMessage(message)
{
}

CSimException::~CSimException() throw()
{
}

const char * CSimException::what() const throw()
{
    return exceptionMessage.c_str();
}

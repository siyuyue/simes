// **********************************************
// CSourceBase Class:
// - Inherits from CPort and CTiming
// - Represents a power source in HEES system
// - Abstract class, used to derive all other source classes
// **********************************************
#pragma once

#include "config.h"
#include "Port.h"
#include "Component.h"

class CSourceBase : public CPort, public CComponent
{
private:
public:
	CSourceBase(void);
	virtual ~CSourceBase(void);
    static CSourceBase* Create(const string &derivedType);		// Called by the parser to create a new source
};

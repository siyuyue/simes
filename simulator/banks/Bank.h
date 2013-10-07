// **********************************************
// CBankBase Class:
// - Inherits from CPort and CTiming
// - Represents a storage bank in HEES system
// - Abstract class, used to derive all other bank classes
// **********************************************
#pragma once

#include "config.h"
#include "Component.h"
#include "Port.h"
#include "Component.h"

class CBankBase : public CPort, public CComponent
{
private:
public:
	CBankBase(void);
	virtual ~CBankBase(void);
    virtual double GetStateofCharge() const = 0;					// Get state of charge
    static CBankBase* Create(const string &derivedType);			// Called by the parser to create a new bank
};

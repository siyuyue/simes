// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CLoadBase Class:
// - Inherits from CPort and CComponent
// - Represents a load in HEES system
// - Abstract class, used to derive all other load classes
// **********************************************
#pragma once
#include "config.h"
#include "Port.h"
#include "Component.h"

class CLoadBase : public CPort, public CComponent
{
private:
public:
	CLoadBase(void);
	virtual ~CLoadBase(void);
    virtual double PortDefaultVoltage(double time) const;				// Default load voltage
    virtual double PortDefaultCurrent(double time) const = 0;			// Default load current
    static CLoadBase* Create(const string &derivedType);                // Called by the parser to create a new load
};

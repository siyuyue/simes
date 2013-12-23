// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CSourceBase Class:
// - Inherits from CPort and CComponent
// - Represents a power source in HEES system
// - Abstract class, used to derive all other source classes
// **********************************************
#pragma once

#include <boost/function.hpp>
#include <boost/functional/factory.hpp>
#include <string>
#include <map>

#include "config.h"
#include "Port.h"
#include "Component.h"

using namespace std;

class CSourceBase : public CPort, public CComponent
{
private:
    static map<string, boost::function<CSourceBase*()> > factories;
public:
	CSourceBase(void);
	virtual ~CSourceBase(void);
    static CSourceBase* Create(const string &derivedType);		// Called by the parser to create a new source
    static void Initialize();
};

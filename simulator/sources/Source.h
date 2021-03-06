// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
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
#include "core/config.h"
#include "core/Component.h"
#include "core/Port.h"

using namespace std;

class CSourceBase : public CPort, public CComponent {
private:
    static map<string, boost::function<CSourceBase*()> > factories;
public:
	CSourceBase(void);
	virtual ~CSourceBase(void);
    static CSourceBase* Create(const string &derivedType);		// Called by the parser to create a new source
    static void Initialize();
};

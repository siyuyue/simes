// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CLoadBase Class:
// - Inherits from CPort and CComponent
// - Represents a load in HEES system
// - Abstract class, used to derive all other load classes
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

class CLoadBase : public CPort, public CComponent {
private:
    static map<string, boost::function<CLoadBase*()> > factories;
public:
	CLoadBase(void);
	virtual ~CLoadBase(void);
    virtual double PortDefaultVoltage(double time) const;				// Default load voltage
    virtual double PortDefaultCurrent(double time) const = 0;			// Default load current
    static CLoadBase* Create(const string &derivedType);                // Called by the parser to create a new load
    static void Initialize();
};

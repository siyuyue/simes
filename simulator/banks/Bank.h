// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CBankBase Class:
// - Inherits from CPort and CComponent
// - Represents a storage bank in HEES system
// - Abstract class, used to derive all other bank classes
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

class CBankBase : public CPort, public CComponent
{
private:
    static map<string, boost::function<CBankBase*()> > factories;
public:
	CBankBase(void);
	virtual ~CBankBase(void);
    virtual double GetStateofCharge() const = 0;					// Get state of charge
    static void Initialize();
    static CBankBase* Create(const string &derivedType);			// Called by the parser to create a new bank
};

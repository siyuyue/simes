// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
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
#include "core/config.h"
#include "core/Component.h"
#include "core/Port.h"

using namespace std;

class CBankBase : public CPort, public CComponent {
private:
    static map<string, boost::function<CBankBase*()> > factories;
public:
	CBankBase(void);
	virtual ~CBankBase(void);
    virtual double GetStateofCharge() const = 0;					// Get state of charge
    static void Initialize();
    static CBankBase* Create(const string &derivedType);			// Called by the parser to create a new bank
};

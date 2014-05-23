// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CSensor class
// Used to output results.
// **********************************************
#pragma once

#include <string>
#include <iostream>
#include "core/Property.h"

using namespace std;

class CSensor {
private:
	string _target;
	string _propertyName;
	CProperty *_property;
public:
    CSensor(const string &target, const string &propertyName, CProperty *property);
	~CSensor(void);
	string GetPropertyValue() const;
	string GetTargetPropertyString() const;
};


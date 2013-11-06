// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CSensor class
// Used to output results.
// **********************************************
#pragma once

#include <string>
#include <iostream>

using namespace std;

class CSensor
{
private:
	string _target;
	string _property;
	double *_pValue;
public:
    CSensor(const string &target, const string &prop);
	~CSensor(void);
	double Value();
	void SetPointer(double *pValue);
	bool IsPointerSet();
	string GetTargetPropertyString();
};


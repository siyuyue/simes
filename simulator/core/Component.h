// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CComponent class:
// - Abstract class, defines the common timing interface
// - SetProperty and GetProperfy interface to set and get values
// **********************************************
#pragma once

#include <map>
#include <string>
#include <sstream>

#include "config.h"
#include "Property.h"
#include "Sensor.h"

using namespace std;

class CSimulator;

class CComponent
{
private:
    string _name;                                                       // Unique identifier
    CSimulator *_pSimulator;                                            // Reference to the parent Simulator class
    map<string, CProperty*> _properties;                                // Store all properties
protected:
    void _AddProperty(CProperty *property);
public:
    CComponent(void);
	virtual ~CComponent(void);
    string GetName() const;
    void SetName(const string &name);
    CSimulator* GetSimulator() const;
    void SetSimulator(CSimulator *pSimulator);
	// Timing related interface
	virtual void Reset() = 0;											// Reset
    virtual double NextTimeStep(double time, int precision) const = 0 ;	// Return the next time step for simulation
	virtual void TimeElapse(double time, double timeElapsed) = 0;		// Simulate for elapsed time, start from "time"
	// Property interface
    virtual bool CheckIntegrity() const;                                // Check whether the properties are properly set
    bool SetProperty(const string &propertyName, const string &value);  // Set the property identified by name
    bool SetPropertyInitial(const string &propertyName, const string &value);  // Set the initial property identified by name
    bool GetProperty(const string &propertyName, string &value);        // Get the property identified by name
    CProperty* SetSensor(const string &propertyName);                   // Set up sensor to monitor values
};

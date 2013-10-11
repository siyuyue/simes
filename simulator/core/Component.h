// **********************************************
// CComponent class:
// - Abstract class, defines the common timing interface
// - SetProperty and GetProperfy interface to set and get values
// **********************************************
#pragma once

#include <string>
#include <sstream>
#include "config.h"
#include "Sensor.h"

using namespace std;

class CSimulator;

class CComponent
{
private:
    string _name;                                                       // Unique identifier
    CSimulator *_pSimulator;                                            // Reference to the parent Simulator class
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
    virtual void CheckIntegrity() const;                                // Check whether the properties are properly set. Throw an exception when not.
    virtual bool SetProperty(const string &name, const string &value);	// Set the property identified by name
    virtual string GetProperty(const string &name) const;				// Get the property identified by name
	// Output interface
    virtual bool SetSensor(const string &name, CSensor &sensor);		// Set up sensor to monitor values
};

// Conversion from string to int/float/double
template <typename T>
T FromString(const string& str)
{
	T t;
	istringstream ss(str);
	ss >> t;
	return t;
}

// Conversion from int/float/double to string
template <typename T>
string ToString(const T t)
{
	ostringstream ss;
	ss << t;
	return ss.str();
}

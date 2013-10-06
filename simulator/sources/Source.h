// **********************************************
// Source.h
// Definition of all source classes
// **********************************************
#pragma once
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include "config.h"
#include "Component.h"
#include "Converter.h"

using namespace std;

// **********************************************
// CSourceBase Class:
// - Inherits from CPort and CTiming
// - Represents a power source in HEES system
// - Abstract class, used to derive all other source classes
// **********************************************
class CSourceBase : public CPort, public CComponent
{
private:
public:
	CSourceBase(void);
	virtual ~CSourceBase(void);
    static CSourceBase* Create(const string &derivedType);		// Called by the parser to create a new source
};

// **********************************************
// CIdealVoltageSource Class:
// - Inherits from CSourceBase
// - Ideal source with constant terminal voltage
// **********************************************
class CIdealVoltageSource:public CSourceBase
{
private:
	double _voltage;												// Terminal voltage
	double _consumption;											// Record the overall consumption
public:
	CIdealVoltageSource(void);
	virtual ~CIdealVoltageSource();
	double GetEnergyConsumption();									// Get energy consumption

    virtual double PortVoltage(double time, double current) const;	// Inherited from CPort
    virtual double MaxOutPortCurrent(double time) const;
	virtual void Reset();											// Inherited from CTiming
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string &value);
    virtual string GetProperty(const string &name) const;
    virtual bool SetSensor(const string &name, CSensor &sensor);
};

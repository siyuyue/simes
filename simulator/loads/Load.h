// **********************************************
// Load.h
// Definition of all load classes
// **********************************************
#pragma once
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include "config.h"
#include "Port.h"
#include "Component.h"
#include "Converter.h"

using namespace std;

// **********************************************
// CLoadBase Class:
// - Inherits from CPort and CTiming
// - Represents a load in HEES system
// - Abstract class, used to derive all other load classes
// **********************************************
class CLoadBase : public CPort, public CComponent
{
private:
public:
	CLoadBase(void);
	virtual ~CLoadBase(void);
    virtual double PortDefaultVoltage(double time) const;				// Default load voltage
    virtual double PortDefaultCurrent(double time) const = 0;			// Default load current
    static CLoadBase* Create(const string &derivedType);                      // Called by the parser to create a new load
};

// **********************************************
// CConstantLoad Class:
// - Inherits from CLoadBase
// - Load with constant power demand
// **********************************************
class CConstantLoad:public CLoadBase
{
private:
	double _voltage;												// Load voltage
	double _current;												// Load Current
	double _consumption;											// Record the overall consumption
public:
	CConstantLoad(void);
	virtual ~CConstantLoad();
    double GetConsumption() const;									// Get consumption

    virtual double PortDefaultCurrent(double time) const;			// Inherited from CLoadBase
    virtual double PortVoltage(double time, double current) const;	// Inherited from CPort
    virtual double MaxOutPortCurrent(double time) const;
	virtual void Reset();											// Inherited from CTiming
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string &value);
    virtual string GetProperty(const string &name) const;
    virtual bool SetSensor(const string &name, CSensor &sensor);
};

// **********************************************
// CProfileLoad Class:
// - Inherits from CLoadBase
// - Load profile given in a input file
// **********************************************
class CProfileLoad:public CLoadBase
{
private:
	double _voltage;												// Load voltage
	vector<double> _time;											// Time
	vector<double> _current;										// Load Current
	int _idx;														// idx of current <time, current> pair
	double _consumption;											// Record the overall consumption
	double _powerNow;												// Current power consumption
public:
	CProfileLoad(void);
	virtual ~CProfileLoad();
    double GetConsumption() const;									// Get consumption
				
    virtual double PortDefaultCurrent(double time) const;			// Inherited from CLoadBase
    virtual double PortVoltage(double time, double current) const;	// Inherited from CPort
    virtual double MaxOutPortCurrent(double time) const;
    virtual void Reset();											// Inherited from CTiming
    virtual double NextTimeStep(double time, int precision) const;
    virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string &value);
    virtual string GetProperty(const string &name) const;
    virtual bool SetSensor(const string &name, CSensor &sensor);
    virtual void CheckIntegrity() const;
};

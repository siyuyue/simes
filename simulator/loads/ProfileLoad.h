// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CProfileLoad Class:
// - Inherits from CLoadBase
// - Load profile given in a input file
// **********************************************
#pragma once

#include <vector>
#include "Load.h"

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
    virtual void Reset();											// Inherited from CComponent
    virtual double NextTimeStep(double time, int precision) const;
    virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string &value);
    virtual string GetProperty(const string &name) const;
    virtual bool SetSensor(const string &name, CSensor &sensor);
    virtual void CheckIntegrity() const;
};
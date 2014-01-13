// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CSimpleBattery Class:
// - Inherits from CBankBase
// - Ideal battery with constant open circuit voltage and internal resister
// **********************************************
#pragma once

#include "config.h"
#include "Bank.h"

class CSimpleBattery:public CBankBase
{
private:
	double _capacity;												// Capacity of the bank in Ah
	double _stateOfCharge;											// State of charge, between 0 and 1
	double _openCircuitVoltage;										// Open circuit voltage
	double _internalResistance;										// Internal resistance
    double _consumption;
public:
	CSimpleBattery(void);
	virtual ~CSimpleBattery();
    double GetOpenCircuitVoltage() const;							// Get open circuit voltage
	double Charge(double duration, double current);					// Charge the bank with current for duration
	double Discharge(double duration, double current);				// Discharge the bank with current for duration
    virtual double GetStateofCharge() const;						// Inherited from CBankBase
    virtual double PortVoltage(double time, double current) const;	// Inherited from CPort
    virtual double MaxOutPortCurrent(double time) const;
	virtual void Reset();											// Inherited from CComponent
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
    bool GetClosedCircuitVoltage(string& s);
};
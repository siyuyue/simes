// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CSuperCap Class:
// - Inherits from CBankBase
// - Ideal capacitor with serial connected internal resistance and self discharge
// **********************************************
#pragma once

#include "core/config.h"
#include "banks/Bank.h"

class CSuperCap:public CBankBase {
private:
	double _capacitance;
	double _openCircuitVoltage;
	double _selfDischargeRate;
	double _serialResistance;
	double _minVoltage;
	double _maxVoltage;
	double _maxPower;
    double _consumption;

public:
	CSuperCap(void);
	~CSuperCap(void);
	double GetCapacitance();
    virtual double GetStateofCharge() const;						// Inherited from CBankBase
    virtual double PortVoltage(double time, double current) const;	// Inherited from CPort
    virtual double MaxOutPortCurrent(double time) const;
    virtual void Reset();											// Inherited from CComponent
    virtual double NextTimeStep(double time, int precision) const;
    virtual void TimeElapse(double time, double timeElapsed);
};
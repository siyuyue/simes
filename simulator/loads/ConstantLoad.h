// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CConstantLoad Class:
// - Inherits from CLoadBase
// - Load with constant power demand
// **********************************************
#pragma once

#include "loads/Load.h"

class CConstantLoad:public CLoadBase {
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
	virtual void Reset();											// Inherited from CComponent
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
};
// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CGrid Class:
// - Inherits from CBankBase since it's bi-directional (allow selling back)
// **********************************************
#pragma once

#include "core/config.h"
#include "banks/Bank.h"

class CGrid:public CBankBase {
private:
	double _voltage;
    double _maxPortCurrent;
    double _consumption;
public:
	CGrid(void);
	~CGrid(void);
    virtual double GetStateofCharge() const;						// Inherited from CBankBase
    virtual double PortVoltage(double time, double current) const;	// Inherited from CPort
    virtual double MaxOutPortCurrent(double time) const;
    virtual void Reset();											// Inherited from CComponent
    virtual double NextTimeStep(double time, int precision) const;
    virtual void TimeElapse(double time, double timeElapsed);
};
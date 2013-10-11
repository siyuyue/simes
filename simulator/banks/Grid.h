// **********************************************
// CGrid Class:
// - Inherits from CBankBase since it's bi-directional (allow selling back)
// **********************************************
#pragma once

#include "config.h"
#include "Bank.h"

class CGrid:public CBankBase
{
private:
	double _voltage;
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
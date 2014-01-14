// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CIdealVoltageSource Class:
// - Inherits from CSourceBase
// - Ideal source with constant terminal voltage
// **********************************************
#include "sources/Source.h"

class CIdealVoltageSource:public CSourceBase {
private:
	double _voltage;												// Terminal voltage
	double _generation;											// Record the overall consumption
public:
	CIdealVoltageSource(void);
	virtual ~CIdealVoltageSource();
	double GetEnergyGeneration();									// Get energy consumption

    virtual double PortVoltage(double time, double current) const;	// Inherited from CPort
    virtual double MaxOutPortCurrent(double time) const;
	virtual void Reset();											// Inherited from CComponent
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
};
// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CSuperCap Class:
// - Inherits from CBankBase
// - Ideal capacitor with serial connected internal resistance and self discharge
// **********************************************
#pragma once

#include "config.h"
#include "Bank.h"

class CSuperCap:public CBankBase
{
private:
	double _capacitance;
	double _openCircuitVoltage;
	double _selfDischargeRate;
	double _seriesResistace;
	double _minVoltage;
	double _maxVoltage;
	double _maxPower;

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
    virtual bool SetProperty(const string &name, const string &value);
    virtual string GetProperty(const string &name) const;
    virtual bool SetSensor(const string &name, CSensor &sensor);
};
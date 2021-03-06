// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CLeadAcidBattery Class:
// - Inherits from CBankBase
// - One type of lead-acid battery model
// **********************************************
#pragma once

#include "core/config.h"
#include "banks/Bank.h"

class CLeadAcidBattery:public CBankBase {
private:
	double _capacity;												// The nominal energy capacity of the battery in Ah
	double _stateOfCharge;
	double _peukertCharge;											// Rate capacity effect for charging
	double _peukertDischarge;										// Rate capacity effect for discharging
	double _b11,_b12,_b13,_b14,_b15;								// For OCV
	double _b21,_b22,_b23;											// For IR Discharging
	double _b31,_b32,_b33,_b34,_b35,_b36,_b37;						// For IR Charging
	int _mBank ;													// m*n bank: m in parallel, n in series
	int _nBank ;
	double _ageFactor;

	double _ccv;
    double _consumption;
public:
	CLeadAcidBattery(void);
	~CLeadAcidBattery(void);
    double GetOpenCircuitVoltage() const;
    virtual double GetStateofCharge() const;						// Inherited from CBankBase
    virtual double PortVoltage(double time, double current) const;	// Inherited from CPort
    virtual double MaxOutPortCurrent(double time) const;
    virtual void Reset();											// Inherited from CComponent
    virtual double NextTimeStep(double time, int precision) const;
    virtual void TimeElapse(double time, double timeElapsed);
};
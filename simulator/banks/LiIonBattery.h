// **********************************************
// CLiIonBattery Class:
// - Inherits from CBankBase
// - One type of Li-ion battery model
// **********************************************
#pragma once

#include "config.h"
#include "Bank.h"

class CLiIonBattery:public CBankBase
{
private:
	double _capacity;												// The nominal energy capacity of the battery: Ah
	double _stateOfCharge;
	double _peukertCharge;											// Rate capacity effect for charging
	double _peukertDischarge;										// Rate capacity effect for discharging
    double _b11,_b12,_b13,_b14,_b15,_b16,_b21,_b22,_b23;            // Battery empirically parameters
	int _mBank ;													// n*m bank: n in parallel, m in series
	int _nBank ;
	
	double _ccv;
	double _consumption;
public:
	CLiIonBattery(void);
	~CLiIonBattery(void);
    double GetOpenCircuitVoltage() const;
    double GetConsumption() const;
    virtual double GetStateofCharge() const;						// Inherited from CBankBase
    virtual double PortVoltage(double time, double current) const;	// Inherited from CPort
    virtual double MaxOutPortCurrent(double time) const;
	virtual void Reset();											// Inherited from CTiming
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string& value);
    virtual string GetProperty(const string &name) const;
    virtual bool SetSensor(const string &name, CSensor &sensor);
};
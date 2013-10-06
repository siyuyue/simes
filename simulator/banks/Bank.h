// **********************************************
// Bank.h
// Definition of all bank classes
// **********************************************
#pragma once

#include <cstdlib>
#include <vector>
#include <cmath>
#include "config.h"
#include "Converter.h"
#include "Port.h"
#include "Component.h"

using namespace std;

// **********************************************
// CBankBase Class:
// - Inherits from CPort and CTiming
// - Represents a storage bank in HEES system
// - Abstract class, used to derive all other bank classes
// **********************************************
class CBankBase : public CPort, public CComponent
{
private:
public:
	CBankBase(void);
	virtual ~CBankBase(void);
    virtual double GetStateofCharge() const = 0;					// Get state of charge
    static CBankBase* Create(const string &derivedType);			// Called by the parser to create a new bank
};

// **********************************************
// CSimpleBattery Class:
// - Inherits from CBankBase
// - Ideal storage bank with constant open circuit voltage and internal resister
// **********************************************
class CSimpleBattery:public CBankBase
{
private:
	double _capacity;												// Capacity of the bank in Ah
	double _stateOfCharge;											// State of charge, between 0 and 1
	double _openCircuitVoltage;										// Open circuit voltage
	double _internalResistance;										// Internal resistance
public:
	CSimpleBattery(void);
	virtual ~CSimpleBattery();
    double GetOpenCircuitVoltage() const;							// Get open circuit voltage
	double Charge(double duration, double current);					// Charge the bank with current for duration
	double Discharge(double duration, double current);				// Discharge the bank with current for duration
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

// **********************************************
// CLiIonBattery Class:
// - Inherits from CBankBase
// - One type of Li-ion battery model
// **********************************************
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

// **********************************************
// CLeadAcidBattery Class:
// - Inherits from CBankBase
// - One type of Li-ion battery model
// **********************************************
class CLeadAcidBattery:public CBankBase
{
private:
	double _capacity;												// The nominal energy capacity of the battery: Ah
	double _stateOfCharge;
	double _peukertCharge;											// Rate capacity effect for charging
	double _peukertDischarge;										// Rate capacity effect for discharging
	double _b11,_b12,_b13,_b14,_b15;								// For OCV
	double _b21,_b22,_b23;											// For IR Discharging
	double _b31,_b32,_b33,_b34,_b35,_b36,_b37;						// For IR Charging
	int _mBank ;													// n*m bank: n in parallel, m in series
	int _nBank ;
	double _ageFactor;

	double _ccv;
public:
	CLeadAcidBattery(void);
	~CLeadAcidBattery(void);
    double GetOpenCircuitVoltage() const;
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

// **********************************************
// CSuperCap Class:
// - Inherits from CBankBase
// - Ideal capacitor with serial connected internal resistance and self discharge
// **********************************************
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
    virtual void Reset();											// Inherited from CTiming
    virtual double NextTimeStep(double time, int precision) const;
    virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string &value);
    virtual string GetProperty(const string &name) const;
    virtual bool SetSensor(const string &name, CSensor &sensor);
};

// **********************************************
// CGrid Class:
// - Inherits from CBankBase
// - Bi-directional
// **********************************************
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
    virtual void Reset();											// Inherited from CTiming
    virtual double NextTimeStep(double time, int precision) const;
    virtual void TimeElapse(double time, double timeElapsed);
};


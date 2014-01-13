// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// Definition of CCTIPort and CCTI classes
// **********************************************
#pragma once

#include "config.h"
#include "Port.h"
#include "Component.h"
#include "Converter.h"
#include <vector>
#include <cmath>

using namespace std;

// **********************************************
// CCTIPort class:
// - Inherited from CPort
// - One CTI can have multiple ports
// ********************************************** 
class CCTIPort : public CPort {
private:
	double *_pCTIVoltage;
	int _CTIID;
	double _maxPower;
public:
	CCTIPort(int id, double *pVoltage);
	virtual ~CCTIPort();										// Inherited from CPort
    virtual double PortVoltage(double time, double current) const;
    virtual double MaxOutPortCurrent(double time) const;
};

// **********************************************
// CCTI class:
// - Inherited from CTiming
// - Modeled as an ideal capacitor
// ********************************************** 
class CCTI : public CComponent {
private:
	double _voltage;											// Current voltage
	double _targetVoltage;										// Target voltage
	double _capacitance;										// Capacitance of CTI
	CConverterBase *_pVoltageRegulator;							// Converter serve as the voltage regulator of the CTI
	bool _isVoltageRegulatorPortA;								// Is CTI Port connected to portA of voltage regulator
	int _myID;													// ID of CTI
	vector<CCTIPort*> _pPorts;									// CTI ports
public:
	CCTI(void);
	virtual ~CCTI(void);
	void SetID(int myId);										// Set ID
	void SetTargetVoltage(double voltage);						// Set target voltage
	void FindRegulatorCurrent(double time);						// Determine the current of the voltage regulator
	bool SetVoltageRegulator(CConverterBase *pRegulator);		// Set the voltage regulator
    double GetVoltage() const;									// Get voltage
    double GetTargetVoltage() const;							// Get target voltage
    double GetCapacitance() const;								// Get capacitance
	CPort * NewPort();											// Create a new CTI port
    CPort * GetPort(int id) const;								// Get id-th CTI port
	static CCTI* Create();										// Called by the parser to create a new CTI
    static void Initialize();
	
    virtual void Reset();										// Inherited from CComponent
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
    virtual bool CheckIntegrity() const;
    bool SetVoltageRegulatorByName(const string&s);
    bool GetVoltageRegulatorName(string&s);
};


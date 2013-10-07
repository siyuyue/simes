// **********************************************
// CConverterBase class:
// - Inherited from CTiming
// - Join two CPort
// - Abstract class
// **********************************************
#pragma once

#include "config.h"
#include "Component.h"
#include "Port.h"

class CConverterBase : public CComponent
{
protected:
	CPort *_pPortA;
	CPort *_pPortB;
	double _outputCurrent;
	double _inputCurrent;
	bool _isOutputSet;
	bool _isPortAOutput;
	int _maxIter;
public:
	CConverterBase(void);
	virtual ~CConverterBase(void);
	void SetPortA(CPort *pPort);
	void SetPortB(CPort *pPort);
    bool CanPortBeOutput(bool isPortA) const;
	void SetPortOutputCurrent(double time, bool isPortA, double current);
	void SetPortInputCurrent(double time, bool isPortA, double current);
	virtual void Solve(double time);
    virtual double GetMaxInputPowerCurrent(double time, bool isPortA) const;
    virtual double FindOutputCurrent(double time, bool isOutputPortA, double inputVoltage, double outputVoltage, double inputCurrent) const = 0;
    virtual double FindInputCurrent(double time, bool isInputPortA, double inputVoltage, double outputVoltage, double outputCurrent) const = 0;
    virtual string GetProperty(const string &name) const;
    virtual bool SetSensor(const string &name, CSensor &sensor);
    static CConverterBase* Create(const string &derivedType);
};

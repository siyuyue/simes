// **********************************************
// CSimpleConverter class:
// - Inherited from CConverterBase
// - Constant efficiency
// **********************************************
#pragma once

#include "config.h"
#include "Converter.h"

class CSimpleConverter : public CConverterBase
{
private:
	double _efficiency;
public:
	CSimpleConverter(void);
	virtual ~CSimpleConverter();
    virtual double FindOutputCurrent(double time, bool isOutputPortA, double inputVoltage, double outputVoltage, double inputCurrent) const;
    virtual double FindInputCurrent(double time, bool isInputPortA, double inputVoltage, double outputVoltage, double outputCurrent) const;
	virtual void Reset();
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string &value);
    virtual string GetProperty(const string &name) const;
};
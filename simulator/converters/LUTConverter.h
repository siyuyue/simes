// **********************************************
// CLUTConverter class:
// - Inherited from CConverterBase
// - Efficiency data from a look up table
// **********************************************
#pragma once

#include <string>
#include "config.h"
#include "Converter.h"
#include "LookUpTable.h"

class CLUTConverter : public CConverterBase
{
private:
    CLookUpTable *_pLUT;
	bool _isInputCurrent;

public:
	CLUTConverter(void);
	virtual ~CLUTConverter();
    virtual double FindOutputCurrent(double time, bool isOutputPortA, double inputVoltage, double outputVoltage, double inputCurrent) const;
    virtual double FindInputCurrent(double time, bool isInputPortA, double inputVoltage, double outputVoltage, double outputCurrent) const;
    virtual void Reset();
    virtual double NextTimeStep(double time, int precision) const;
    virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string &value);
    virtual void CheckIntegrity() const;
};
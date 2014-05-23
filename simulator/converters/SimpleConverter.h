// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CSimpleConverter class:
// - Inherited from CConverterBase
// - Constant efficiency
// **********************************************
#pragma once

#include "converters/Converter.h"
#include "core/config.h"

class CSimpleConverter : public CConverterBase {
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
};
// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CLUTConverter class:
// - Inherited from CConverterBase
// - Efficiency data from a look up table
// **********************************************
#pragma once

#include <string>
#include "core/config.h"
#include "core/LookUpTable.h"
#include "converters/Converter.h"

class CLUTConverter : public CConverterBase {
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
    virtual bool CheckIntegrity() const;
    bool SetLUT(const string &s, bool isInputCurrent);
};
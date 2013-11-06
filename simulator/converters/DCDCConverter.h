// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CDCDCConverter class:
// - Inherited from CConverterBase
// - Use an efficiency model in a previous paper
// **********************************************
#pragma once

#include "config.h"
#include "Converter.h"

class CDCDCConverter : public CConverterBase
{
private:
	// -------- parameter sets ------------
	double _scale;
	double _fixed;
	double _vari_buck;
	double _vari_boost;

	double _L_f;
	double _R_l;
	double _R_sw1;
	double _R_sw2;
	double _R_sw3;
	double _R_sw4;
	double _I_cont;
	double _Q_sw1;
	double _Q_sw2;
	double _Q_sw3;
	double _Q_sw4;
	double _R_c;
	double _F_s;

public:
	CDCDCConverter(void);
	virtual ~CDCDCConverter();
    virtual double FindOutputCurrent(double time, bool isOutputPortA, double inputVoltage, double outputVoltage, double inputCurrent) const;
    virtual double FindInputCurrent(double time, bool isInputPortA, double inputVoltage, double outputVoltage, double outputCurrent) const;
    virtual void Reset();
    virtual double NextTimeStep(double time, int precision) const;
    virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string &value);
};
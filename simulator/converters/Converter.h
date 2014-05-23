// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CConverterBase class:
// - Inherited from CComponent
// - Join two CPort
// - Abstract class
// **********************************************
#pragma once

#include <boost/function.hpp>
#include <boost/functional/factory.hpp>
#include <string>
#include <map>
#include "core/config.h"
#include "core/Component.h"
#include "core/Port.h"

using namespace std;

class CConverterBase : public CComponent {
private:
    static map<string, boost::function<CConverterBase*()> > factories;
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
    static CConverterBase* Create(const string &derivedType);
    static void Initialize();
};

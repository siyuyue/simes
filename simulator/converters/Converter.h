// **********************************************
// Converter.h
// Definition of all converter classes
// **********************************************
#pragma once
#include <string>
#include <vector>
#include <cmath>
#include "config.h"
#include "Component.h"
#include "Port.h"
#include "LookUpTable.h"

using namespace std;

// **********************************************
// CConverterBase class:
// - Inherited from CTiming
// - Join two CPort
// - Abstract class
// **********************************************
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

// **********************************************
// CSimpleConverter class:
// - Inherited from CConverterBase
// - Constant efficiency
// **********************************************
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

// **********************************************
// CDCDCConverter class:
// - Inherited from CConverterBase
// - Use an efficiency model in a previous paper
// **********************************************
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

// **********************************************
// CLUTConverter class:
// - Inherited from CConverterBase
// - Efficiency data from a look up table
// **********************************************
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

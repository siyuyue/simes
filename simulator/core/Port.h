// **********************************************
// Port.h
// Definition of CPort base class
// **********************************************
#pragma once
#include "config.h"

class CConverterBase;

enum PortType
{
	LOAD, SOURCE, BANK, CTI
};

// **********************************************
// CPort Class
// Abstract class
// **********************************************
class CPort
{
protected:
	PortType _type;
	bool _inPortMode;
	bool _outPortMode;
	double _portCurrent;
	CConverterBase *_pConverter;
	bool _isPortA;
public:
	CPort(bool inMode, bool outMode, PortType type);
	virtual ~CPort(void);
    bool IsInPort() const;
    bool IsOutPort() const;
    bool IsFloating() const;
    bool IsPortA() const;
    PortType GetType() const;
	void SetConverter(CConverterBase *pConverter, bool isPortA);
    CConverterBase *GetConverter() const;
	void SetPortCurrent(double current);
    double GetPortCurrent() const;
    virtual double PortVoltage(double time, double current) const = 0;
    virtual double MaxOutPortCurrent(double time) const = 0;
};

// **********************************************
// CIdealVoltageSource Class:
// - Inherits from CSourceBase
// - Ideal source with constant terminal voltage
// **********************************************
#include "Source.h"

class CIdealVoltageSource:public CSourceBase
{
private:
	double _voltage;												// Terminal voltage
	double _consumption;											// Record the overall consumption
public:
	CIdealVoltageSource(void);
	virtual ~CIdealVoltageSource();
	double GetEnergyConsumption();									// Get energy consumption

    virtual double PortVoltage(double time, double current) const;	// Inherited from CPort
    virtual double MaxOutPortCurrent(double time) const;
	virtual void Reset();											// Inherited from CComponent
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string &value);
    virtual string GetProperty(const string &name) const;
    virtual bool SetSensor(const string &name, CSensor &sensor);
};
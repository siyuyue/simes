#include "IdealVoltageSource.h"

CIdealVoltageSource::CIdealVoltageSource(void)
{
	_voltage = 5;
}

CIdealVoltageSource::~CIdealVoltageSource()
{
}

void CIdealVoltageSource::Reset()
{
	_consumption = 0;
}

double CIdealVoltageSource::PortVoltage(double time, double current) const
{
	return _voltage;
}

double CIdealVoltageSource::MaxOutPortCurrent(double time) const
{
	return INF;
}

void CIdealVoltageSource::TimeElapse(double time, double timeElapsed)
{
	_consumption += -_portCurrent*_voltage*timeElapsed;
}

double CIdealVoltageSource::GetEnergyConsumption()
{
	return _consumption;
}

double CIdealVoltageSource::NextTimeStep(double time, int precision) const
{
	return INF;
}

bool CIdealVoltageSource::SetProperty(const string &name, const string &value)
{
	if( name == string("voltage") )
	{
		_voltage = FromString<double>(value);
		return true;
	}
	return false;
}

string CIdealVoltageSource::GetProperty(const string &name) const
{
	return string();
}

bool CIdealVoltageSource::SetSensor(const string &name, CSensor &sensor)
{
	if( name == string("consumption") )
	{
		sensor.SetPointer(&_consumption);
		return true;
	}
	if( name == string("current") )
	{
		sensor.SetPointer(&_portCurrent);
		return true;
	}
	return false;
}
#include "ConstantLoad.h"

CConstantLoad::CConstantLoad(void)
{
	_voltage = 1;
	_current = 1;
}

CConstantLoad::~CConstantLoad()
{
}

void CConstantLoad::Reset()
{
	_consumption = 0;
}

double CConstantLoad::PortDefaultCurrent(double time) const
{
	return _current;
}

double CConstantLoad::PortVoltage(double time, double current) const
{
	return _voltage;
}

double CConstantLoad::MaxOutPortCurrent(double time) const
{
	return 0;
}

double CConstantLoad::NextTimeStep(double time, int precision) const
{
	return INF;
}

void CConstantLoad::TimeElapse(double time, double timeElapsed)
{
	_consumption += timeElapsed * _voltage * _portCurrent;
}

double CConstantLoad::GetConsumption() const
{
	return _consumption;
}

bool CConstantLoad::SetProperty(const string &name, const string &value)
{
	if( name == string("voltage") )
	{
		_voltage = FromString<double>(value);
		return true;
	}
	if( name == string("current") )
	{
		_current = FromString<double>(value);
		return true;
	}
	return false;
}

string CConstantLoad::GetProperty(const string &name) const
{
	return string();
}

bool CConstantLoad::SetSensor(const string &name, CSensor &sensor)
{
	if( name == string("consumption") )
	{
		sensor.SetPointer(&_consumption);
		return true;
	}
	return false;
}
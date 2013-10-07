#include <cmath>
#include "SuperCap.h"

CSuperCap::CSuperCap(void)
{
	_capacitance = 1.;
	_openCircuitVoltage = 1.;
	_selfDischargeRate = 0;
	_seriesResistace = 0;
	_minVoltage = 1.;
    _maxVoltage = 50.;
	_maxPower = 100;
}

CSuperCap::~CSuperCap(void)
{

}

double CSuperCap::GetCapacitance()
{
	return _capacitance;
}

double CSuperCap::GetStateofCharge() const
{
	return _openCircuitVoltage / _maxVoltage;
}

double CSuperCap::PortVoltage(double time, double current) const
{
	return _openCircuitVoltage + current * _seriesResistace;
}

double CSuperCap::MaxOutPortCurrent(double time) const
{
	if( _openCircuitVoltage < _minVoltage )
	{
		return 0;
	}
	if( _seriesResistace == 0 )
	{
		return _maxPower / _openCircuitVoltage;
	}
	return _openCircuitVoltage / _seriesResistace;
}

void CSuperCap::Reset()
{
	;
}

double CSuperCap::NextTimeStep(double time, int precision) const
{
	if( _portCurrent == 0 )
	{

		if( _selfDischargeRate == 0 )
		{
			return INF;
		}
		else
		{
			return 1 / _selfDischargeRate;
		}
	}
	double difference = 0.1/(1+precision);
    return difference * _capacitance / abs(_portCurrent);
}

void CSuperCap::TimeElapse(double time, double timeElapsed)
{
	double energyNow = _capacitance * _openCircuitVoltage * _openCircuitVoltage / 2;
	double energyChanged = _portCurrent * _openCircuitVoltage * timeElapsed;
	double selfDischargeRatio = _openCircuitVoltage < _minVoltage ? 1 : exp(- timeElapsed*_selfDischargeRate);
	double energyNew = energyNow*selfDischargeRatio + energyChanged;
	_openCircuitVoltage = sqrt( energyNew * 2 / _capacitance );
	if( _openCircuitVoltage < _minVoltage )
	{
		// ToDo: Maybe a warning here.
	}
	else if( _openCircuitVoltage > _maxVoltage)
	{
		// ToDo: Maybe a warning here.
	}
}

bool CSuperCap::SetProperty(const string &name, const string& value)
{
	if( name == string("voltage") )
	{
		_openCircuitVoltage = FromString<double>(value);
		return true;
	}
	if( name == string("capacitance") )
	{
		_capacitance = FromString<double>(value);
		return true;
	}
	if( name == string("series_resistance") )
	{
		_seriesResistace = FromString<double>(value);
		return true;
	}
	return false;
}

string CSuperCap::GetProperty(const string &name) const
{
	return string();
}

bool CSuperCap::SetSensor(const string &name, CSensor &sensor)
{
	if( name == string("voltage") )
	{
		sensor.SetPointer(&_openCircuitVoltage);
		return true;
	}
	if( name == string("current") )
	{
		sensor.SetPointer(&_portCurrent);
		return true;
	}
	return false;
}
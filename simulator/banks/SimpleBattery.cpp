#include <cmath>
#include "SimpleBattery.h"
#include "SimException.h"

CSimpleBattery::CSimpleBattery(void)
{
    // Default values, use SetProperty interface to modify them
	_capacity = 1;
	_openCircuitVoltage = 5;
	_internalResistance = 0;
}

CSimpleBattery::~CSimpleBattery()
{
}

void CSimpleBattery::Reset()
{
	_stateOfCharge = 1.;
}

double CSimpleBattery::GetStateofCharge() const
{
	return _stateOfCharge;
}

double CSimpleBattery::GetOpenCircuitVoltage() const
{
	return _openCircuitVoltage;
}

double CSimpleBattery::Charge(double duration, double current)
{
	double chargeChange = current*duration;
	_stateOfCharge += chargeChange / (3600*_capacity);
	if( _stateOfCharge > 1 )
	{
		throw CSimException(GetName().c_str(), "State of charge goes above capacity limit.");
	}
	return chargeChange*_openCircuitVoltage;
}

double CSimpleBattery::Discharge(double duration, double current)
{
	double chargeChange = current*duration;
	_stateOfCharge -= chargeChange / (3600*_capacity);
	if( _stateOfCharge <= 0 )
	{
		throw CSimException(GetName().c_str(), "State of charge goes below zero.");
	}
	return chargeChange*_openCircuitVoltage;
}

double CSimpleBattery::NextTimeStep(double time, int precision) const
{
	if(abs(_portCurrent) <= EPS)
		return INF;
	else if(_portCurrent < 0)
		return _stateOfCharge*3600*_capacity/(-_portCurrent);
	else
		return (1-_stateOfCharge)*3600*_capacity/(_portCurrent);
}

double CSimpleBattery::PortVoltage(double time, double current) const
{
	return _openCircuitVoltage - current*_internalResistance;
}

double CSimpleBattery::MaxOutPortCurrent(double time) const
{
	if( _internalResistance == 0 )
	{
		return INF;
	}
	return _openCircuitVoltage / _internalResistance;
}

void CSimpleBattery::TimeElapse(double time, double timeElapsed)
{
	if( _portCurrent < 0 )
	{
		Discharge(timeElapsed,-_portCurrent);
	}
	else if( _portCurrent > 0 )
	{
		Charge(timeElapsed,_portCurrent);
	}
}

bool CSimpleBattery::SetProperty(const string &name, const string& value)
{
	if( name == string("capacity") )
	{
		_capacity = FromString<double>(value);
		return true;
	}
	if( name == string("open_circuit_voltage") )
	{
		_openCircuitVoltage = FromString<double>(value);
		return true;
	}
	if( name == string("internal_resistance") )
	{
		_internalResistance = FromString<double>(value);
		return true;
	}
	return false;
}

string CSimpleBattery::GetProperty(const string &name) const
{
	return string();
}

bool CSimpleBattery::SetSensor(const string &name, CSensor &sensor)
{
	if( name == string("state_of_charge") )
	{
		sensor.SetPointer(&_stateOfCharge);
		return true;
	}
    if( name == string("current") )
    {
        sensor.SetPointer(&_portCurrent);
        return true;
    }
	return false;
}
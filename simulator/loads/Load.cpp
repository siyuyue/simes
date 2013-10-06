// **********************************************
// Load.cpp
// Implementation of all load classes
// **********************************************
#include "Load.h"
#include "SimException.h"
#include "Simulator.h"
#include <iostream>
#include <fstream>

using namespace std;

// **********************************************
// CLoadBase
// **********************************************
CLoadBase::CLoadBase(void) : CPort(true,false,LOAD)
{
}

CLoadBase::~CLoadBase(void)
{
}

double CLoadBase::PortDefaultVoltage(double time) const
{
	return PortVoltage(time, PortDefaultCurrent(time));
}

// **********************************************
// CConstantLoad
// **********************************************
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

// **********************************************
// CProfileLoad
// **********************************************
CProfileLoad::CProfileLoad()
{
	_voltage = 1.;
}

CProfileLoad::~CProfileLoad()
{
}

double CProfileLoad::GetConsumption() const
{
	return _consumption;
}

double CProfileLoad::PortDefaultCurrent(double time) const
{
	return _current[_idx];
}

double CProfileLoad::PortVoltage(double time, double current) const
{
	return _voltage;
}

double CProfileLoad::MaxOutPortCurrent(double time) const
{
	return 0;
}

void CProfileLoad::Reset()
{
	_consumption = 0;
	_idx = 0;
	_powerNow = _voltage * _current[_idx];
}

double CProfileLoad::NextTimeStep(double time, int precision) const
{
	if( _idx+1 != _time.size() )
	{
		return _time[_idx + 1] - time;
	}
	return INF;
}

void CProfileLoad::TimeElapse(double time, double timeElapsed)
{
	_consumption += _powerNow * timeElapsed;
	while( _idx+1 != _time.size() && time + timeElapsed >= _time[_idx + 1] )
	{
		_idx ++;
	}
	_powerNow = _voltage * _current[_idx];
}

bool CProfileLoad::SetProperty(const string &name, const string &value)
{
	if( name == string("voltage") )
	{
		_voltage = FromString<double>(value);
		return true;
	}
	if( name == string("input_file") )
	{
        ifstream inputProfile(string(GetSimulator()->GetPathPrefix() + value).c_str());
		if(!inputProfile)
		{
			return false;
		}
		_time.clear();
		_current.clear();
		while(inputProfile)
		{
			double time, current;
			inputProfile >> time >> current;
			_time.push_back(time);
			_current.push_back(current);
		}
		return true;
	}
	return false;
}

string CProfileLoad::GetProperty(const string &name) const
{
	return string();
}

bool CProfileLoad::SetSensor(const string &name, CSensor &sensor)
{
	if( name == string("consumption") )
	{
		sensor.SetPointer(&_consumption);
		return true;
	}
	if( name == string("power") )
	{
		sensor.SetPointer(&_powerNow);
		return true;
	}
	return false;
}

void CProfileLoad::CheckIntegrity() const
{
    if(_time.empty())
    {
        throw CSimException(GetName().c_str(), "Load profile is not set.");
    }
}

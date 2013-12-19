// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "ProfileLoad.h"
#include "Simulator.h"
#include "SimException.h"

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
	if( name == string("consumption") )
	{
		return ToString<double>(_consumption);
	}
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

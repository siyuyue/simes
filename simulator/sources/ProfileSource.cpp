// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "ProfileSource.h"
#include "Simulator.h"
#include "SimException.h"

CProfileSource::CProfileSource(void)
{
    _voltage = 5;
}

CProfileSource::~CProfileSource()
{
}

void CProfileSource::Reset()
{
    _generation = 0;
    _idx = 0;
    _powerNow = _voltage * _current[_idx];
}

double CProfileSource::PortVoltage(double time, double current) const
{
    if( current >= _current[_idx] )
    {
        return 0;
    }
    return _voltage;
}

double CProfileSource::MaxOutPortCurrent(double time) const
{
    return _current[_idx];
}

void CProfileSource::TimeElapse(double time, double timeElapsed)
{
    _generation += _powerNow * timeElapsed;
    while( _idx+1 != _time.size() && time + timeElapsed >= _time[_idx + 1] )
    {
        _idx ++;
    }
    _powerNow = _voltage * _current[_idx];
}

double CProfileSource::GetEnergyGeneration()
{
    return _generation;
}

double CProfileSource::NextTimeStep(double time, int precision) const
{
    if( _idx+1 != _time.size() )
    {
        return _time[_idx + 1] - time;
    }
    return INF;
}

bool CProfileSource::SetProperty(const string &name, const string &value)
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

string CProfileSource::GetProperty(const string &name) const
{
    return string();
}

bool CProfileSource::SetSensor(const string &name, CSensor &sensor)
{
    if( name == string("generation") )
    {
        sensor.SetPointer(&_generation);
        return true;
    }
    if( name == string("power") )
    {
        sensor.SetPointer(&_powerNow);
        return true;
    }
    return false;
}

void CProfileSource::CheckIntegrity() const
{
    if(_time.empty())
    {
        throw CSimException(GetName().c_str(), "Source profile is not set.");
    }
}

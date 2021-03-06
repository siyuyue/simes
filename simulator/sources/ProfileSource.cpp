// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "core/Simulator.h"
#include "core/SimException.h"
#include "sources/ProfileSource.h"

CProfileSource::CProfileSource(void) {
    _voltage = 5;

    // Add properties
    _AddProperty(new CProperty("voltage", "Source voltage.",
        InvalidSetter,
        boost::bind(SimpleGetter<double>, _1, boost::ref(_voltage)),
        boost::bind(CheckSetter<double>, _1, boost::ref(_voltage), (boost::lambda::_1 > 0))));
    _AddProperty(new CProperty("input_file", "Current profile.",
        InvalidSetter,
        InvalidGetter,
        boost::bind(&CProfileSource::SetCurrentProfile, this, _1)));
    _AddProperty(new CProperty("generation", "Total energy generated by this source.",
        boost::bind(SimpleGetter<double>, _1, boost::ref(_generation))));
    _AddProperty(new CProperty("power", "Power generated.",
        boost::bind(SimpleGetter<double>, _1, boost::ref(_powerNow))));
}

CProfileSource::~CProfileSource() {
}

void CProfileSource::Reset() {
    _generation = 0;
    _idx = 0;
    _powerNow = _voltage * _current[_idx];
}

double CProfileSource::PortVoltage(double time, double current) const {
    if (current > _current[_idx]) {
        return 0;
    }
    return _voltage;
}

double CProfileSource::MaxOutPortCurrent(double time) const {
    return _current[_idx];
}

void CProfileSource::TimeElapse(double time, double timeElapsed) {
    _generation += _powerNow * timeElapsed;
    while (_idx+1 != _time.size() && time + timeElapsed >= _time[_idx + 1]) {
        _idx ++;
    }
    _powerNow = _voltage * _current[_idx];
}

double CProfileSource::GetEnergyGeneration() {
    return _generation;
}

double CProfileSource::NextTimeStep(double time, int precision) const {
    if (_idx+1 != _time.size()) {
        return _time[_idx + 1] - time;
    }
    return INF;
}

bool CProfileSource::CheckIntegrity() const {
    if (_time.empty()) {
        throw CSimException(GetName().c_str(), "Source profile is not set.");
    }
    return true;
}

bool CProfileSource::SetCurrentProfile(const string &s) {
    ifstream inputProfile(string(GetSimulator()->GetPathPrefix() + s).c_str());
    if (!inputProfile) {
        return false;
    }
    _time.clear();
    _current.clear();
    while (inputProfile) {
        double time, current;
        inputProfile >> time >> current;
        _time.push_back(time);
        _current.push_back(current);
    }
    return true;
}

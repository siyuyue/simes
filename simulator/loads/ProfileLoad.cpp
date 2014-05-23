// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "core/Simulator.h"
#include "core/SimException.h"
#include "loads/ProfileLoad.h"

CProfileLoad::CProfileLoad() {
	_voltage = 1.;

	// Add properties
	_AddProperty(new CProperty("voltage", "Load voltage.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_voltage)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_voltage), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("input_file", "Current profile.",
		InvalidSetter,
		InvalidGetter,
		boost::bind(&CProfileLoad::SetCurrentProfile, this, _1)));
	_AddProperty(new CProperty("consumption", "Total energy consumed by this load.",
		boost::bind(SimpleGetter<double>, _1, boost::ref(_consumption))));
	_AddProperty(new CProperty("power", "Power demand.",
		boost::bind(SimpleGetter<double>, _1, boost::ref(_powerNow))));
}

CProfileLoad::~CProfileLoad() {
}

double CProfileLoad::GetConsumption() const {
	return _consumption;
}

double CProfileLoad::PortDefaultCurrent(double time) const {
	return _current[_idx];
}

double CProfileLoad::PortVoltage(double time, double current) const {
	return _voltage;
}

double CProfileLoad::MaxOutPortCurrent(double time) const {
	return 0;
}

void CProfileLoad::Reset() {
	_consumption = 0;
	_idx = 0;
	_powerNow = _voltage * _current[_idx];
}

double CProfileLoad::NextTimeStep(double time, int precision) const {
	if (_idx+1 != _time.size()) {
		return _time[_idx + 1] - time;
	}
	return INF;
}

void CProfileLoad::TimeElapse(double time, double timeElapsed) {
	_consumption += _powerNow * timeElapsed;
	while (_idx+1 != _time.size() && time + timeElapsed >= _time[_idx + 1]) {
		_idx ++;
	}
	_powerNow = _voltage * _current[_idx];
}

bool CProfileLoad::CheckIntegrity() const {
    if (_time.empty()) {
        throw CSimException(GetName().c_str(), "Load profile is not set.");
    }
    return true;
}

bool CProfileLoad::SetCurrentProfile(const string &s) {
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

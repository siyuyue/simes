// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <cmath>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "banks/SimpleBattery.h"
#include "core/SimException.h"

CSimpleBattery::CSimpleBattery(void) {
    // Default values, use SetProperty interface to modify them
	_capacity = 1;
	_openCircuitVoltage = 5;
	_internalResistance = 0;

	// Add properties.
	_AddProperty(new CProperty("capacity", "Capacity in Ah.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_capacity)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_capacity), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("open_circuit_voltage", "Open circuit voltage.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_openCircuitVoltage)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_openCircuitVoltage), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("internal_resistance", "Internal resistance.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_internalResistance)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_internalResistance), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("state_of_charge", "State-of-charge.",
		boost::bind(CheckSetter<double>, _1, boost::ref(_stateOfCharge), (boost::lambda::_1 >= 0 && boost::lambda::_1 <= 1)),
		boost::bind(SimpleGetter<double>, _1, boost::ref(_stateOfCharge))));
	_AddProperty(new CProperty("closed_circuit_voltage", "Closed circuit voltage.",
		boost::bind(&CSimpleBattery::GetClosedCircuitVoltage, this, _1)));
	_AddProperty(new CProperty("consumption", "Total energy drawn from the storage bank.",
		boost::bind(SimpleGetter<double>, _1, boost::ref(_consumption))));
	_AddProperty(new CProperty("current", "Port Current.",
		boost::bind(SimpleGetter<double>, _1, boost::ref(_portCurrent))));
}

CSimpleBattery::~CSimpleBattery() {
}

void CSimpleBattery::Reset() {
	_consumption = 0;
	_stateOfCharge = 1.;
}

double CSimpleBattery::GetStateofCharge() const {
	return _stateOfCharge;
}

double CSimpleBattery::GetOpenCircuitVoltage() const {
	return _openCircuitVoltage;
}

double CSimpleBattery::Charge(double duration, double current) {
	double chargeChange = current*duration;
	_stateOfCharge += chargeChange / (3600 * _capacity);
	if( _stateOfCharge > 1 ) {
		throw CSimException(GetName().c_str(), "State of charge goes above capacity limit.");
	}
	return chargeChange * _openCircuitVoltage;
}

double CSimpleBattery::Discharge(double duration, double current) {
	double chargeChange = current*duration;
	_stateOfCharge -= chargeChange / (3600 * _capacity);
	if( _stateOfCharge <= 0 ) {
		throw CSimException(GetName().c_str(), "State of charge goes below zero.");
	}
	return chargeChange * _openCircuitVoltage;
}

double CSimpleBattery::NextTimeStep(double time, int precision) const {
	if(abs(_portCurrent) <= EPS) {
		return INF;
	} else if(_portCurrent < 0) {
		return _stateOfCharge*3600*_capacity/(-_portCurrent);
	} else {
		return (1-_stateOfCharge)*3600*_capacity/(_portCurrent);
	}
}

double CSimpleBattery::PortVoltage(double time, double current) const {
	return _openCircuitVoltage - current*_internalResistance;
}

double CSimpleBattery::MaxOutPortCurrent(double time) const {
	if( _internalResistance == 0 ) {
		return INF;
	}
	return _openCircuitVoltage / _internalResistance;
}

void CSimpleBattery::TimeElapse(double time, double timeElapsed)
{
	if( _portCurrent < 0 ) {
		_consumption += Discharge(timeElapsed,-_portCurrent);
	}
	else if( _portCurrent > 0 ) {
		_consumption -= Charge(timeElapsed,_portCurrent);
	}
}

bool CSimpleBattery::GetClosedCircuitVoltage(string& s) {
	s = ToString<double>(_openCircuitVoltage + _portCurrent * _internalResistance);
}

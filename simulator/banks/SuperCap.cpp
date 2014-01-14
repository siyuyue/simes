// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <cmath>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "banks/SuperCap.h"
#include "core/SimException.h"

CSuperCap::CSuperCap(void) {
	_capacitance = 1.;
	_openCircuitVoltage = 1.;
	_selfDischargeRate = 0;
	_serialResistance = 0;
	_minVoltage = 0.;
    _maxVoltage = INF;
	_maxPower = 100;

	// Add properties.
	_AddProperty(new CProperty("capacitance", "Capacitance.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_capacitance)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_capacitance), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("serial_resistance", "Serial resistance.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_serialResistance)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_serialResistance), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("min_voltage", "Minimum open circuit voltage.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_minVoltage)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_minVoltage), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("max_voltage", "Maximum open circuit voltage.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_maxVoltage)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_maxVoltage), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("max_power", "Maximum output power.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_maxPower)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_maxPower), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("serial_resistance", "Serial resistance.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_serialResistance)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_serialResistance), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("voltage", "Open circuit voltage.",
		boost::bind(CheckSetter<double>, _1, boost::ref(_openCircuitVoltage), (boost::lambda::_1 > 0)),
		boost::bind(SimpleGetter<double>, _1, boost::ref(_openCircuitVoltage))));
	_AddProperty(new CProperty("consumption", "Total energy drawn from the storage bank.",
		boost::bind(SimpleGetter<double>, _1, boost::ref(_consumption))));
	_AddProperty(new CProperty("current", "Port Current.",
		boost::bind(SimpleGetter<double>, _1, boost::ref(_portCurrent))));
}

CSuperCap::~CSuperCap(void) {
}

double CSuperCap::GetCapacitance() {
	return _capacitance;
}

double CSuperCap::GetStateofCharge() const {
	return _openCircuitVoltage / _maxVoltage;
}

double CSuperCap::PortVoltage(double time, double current) const {
	return _openCircuitVoltage + current * _serialResistance;
}

double CSuperCap::MaxOutPortCurrent(double time) const {
	if( _openCircuitVoltage < _minVoltage ) {
		return 0;
	}
	if( _serialResistance == 0 ) {
		return _maxPower / _openCircuitVoltage;
	}
	return _openCircuitVoltage / _serialResistance;
}

void CSuperCap::Reset() {
	_consumption = 0;
}

double CSuperCap::NextTimeStep(double time, int precision) const {
	if( _portCurrent == 0 ) {
		if( _selfDischargeRate == 0 ) {
			return INF;
		} else {
			return 1 / _selfDischargeRate;
		}
	}
	double difference = 0.1 / (1 + precision);
    return difference * _capacitance / abs(_portCurrent);
}

void CSuperCap::TimeElapse(double time, double timeElapsed) {
	double energyNow = _capacitance * _openCircuitVoltage * _openCircuitVoltage / 2;
	double energyChanged = _portCurrent * _openCircuitVoltage * timeElapsed;
	double selfDischargeRatio = _openCircuitVoltage < _minVoltage ? 1 : exp(- timeElapsed*_selfDischargeRate);
	double energyNew = energyNow*selfDischargeRatio + energyChanged;
	_openCircuitVoltage = sqrt( energyNew * 2 / _capacitance );
	if( _openCircuitVoltage < _minVoltage ) {
		throw CSimException(GetName().c_str(), "Super capacitor voltage goes below minimum.");
	} else if( _openCircuitVoltage > _maxVoltage) {
		throw CSimException(GetName().c_str(), "Super capacitor voltage goes above maximum.");
	}
	_consumption -= energyChanged;
}

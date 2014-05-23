// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <cmath>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "banks/LiIonBattery.h"
#include "core/SimException.h"

CLiIonBattery::CLiIonBattery(void) {
	_stateOfCharge = 1;
	_capacity = 2.6;		// Capacity of each cell in Ah
	_mBank = 1;
	_nBank = 1;
	// Li-ion Battery parameters
	_peukertCharge = 1./1.4;
	_peukertDischarge = 1.05;

	_b11 = -0.67;   _b12 = -16.21;  _b13 = -0.03;
	_b14 = 1.28;    _b15 =  -0.40;  _b16 = 7.55;
	_b21 = 0.05;    _b22 = -4.32;   _b23 = 0.17;

	// Add properties.
	_AddProperty(new CProperty("capacity", "Capacity of a cell in Ah.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_capacity)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_capacity), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("m_bank", "Number of banks in parallel.",
		InvalidSetter,
		boost::bind(SimpleGetter<int>, _1, boost::ref(_mBank)),
		boost::bind(CheckSetter<int>, _1, boost::ref(_mBank), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("n_bank", "Number of banks in series.",
		InvalidSetter,
		boost::bind(SimpleGetter<int>, _1, boost::ref(_nBank)),
		boost::bind(CheckSetter<int>, _1, boost::ref(_nBank), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("state_of_charge", "State-of-charge.",
		boost::bind(CheckSetter<double>, _1, boost::ref(_stateOfCharge), (boost::lambda::_1 >= 0 && boost::lambda::_1 <= 1)),
		boost::bind(SimpleGetter<double>, _1, boost::ref(_stateOfCharge))));
	_AddProperty(new CProperty("closed_circuit_voltage", "Closed circuit voltage.",
		boost::bind(SimpleGetter<double>, _1, boost::ref(_ccv))));
	_AddProperty(new CProperty("consumption", "Total energy drawn from the storage bank.",
		boost::bind(SimpleGetter<double>, _1, boost::ref(_consumption))));
	_AddProperty(new CProperty("current", "Port Current.",
		boost::bind(SimpleGetter<double>, _1, boost::ref(_portCurrent))));
}

CLiIonBattery::~CLiIonBattery(void) {
}

void CLiIonBattery::Reset() {
	_stateOfCharge = 1;
	_consumption = 0;
}

double CLiIonBattery::GetStateofCharge() const {
	return _stateOfCharge;
}

double CLiIonBattery::GetConsumption() const {
	return _consumption;
}

double CLiIonBattery::GetOpenCircuitVoltage() const {
	return _nBank * ( _b11*exp(_b12*_stateOfCharge)+_b13*_stateOfCharge*_stateOfCharge*_stateOfCharge+_b14*_stateOfCharge*_stateOfCharge+_b15*_stateOfCharge+_b16 ) / 2;
}

double CLiIonBattery::MaxOutPortCurrent(double time) const {
    double Rs;
	Rs = (_b21*exp(_b22*_stateOfCharge)+_b23 ) / 2;
    return ( GetOpenCircuitVoltage() ) * _mBank / Rs;
}


double CLiIonBattery::PortVoltage(double time, double current) const {
    double Rs;
	Rs = (_b21*exp(_b22*_stateOfCharge)+_b23 ) / 2;
    return _nBank * ( GetOpenCircuitVoltage()/_nBank + Rs*current/_mBank );
}

void CLiIonBattery::TimeElapse(double time, double timeElapsed) {
	//Update the charge of Cts and Ctl
	//positive current: discharge
	//Don't use it unless the discharge actually happens
	double current = _portCurrent;
	double current_eff;
    double Rs;
	Rs = (_b21*exp(_b22*_stateOfCharge)+_b23 ) / 2;
	if (abs(current/_mBank) > _capacity/20.) {
		if (current/_mBank > 0) {
			current_eff = pow((abs(current/_mBank/(_capacity/20.))),_peukertCharge) * _capacity/20.;
		} else {
			current_eff = -pow((abs(current/_mBank/(_capacity/20.))),_peukertDischarge) * _capacity/20.;
		}
	} else {
		current_eff = current/_mBank;
	}
	_stateOfCharge += (current_eff)*timeElapsed/(_capacity*3600);
	if( _stateOfCharge > 1 ) {
		throw CSimException(GetName().c_str(), "State of charge goes above capacity limit.");
	}
	if( _stateOfCharge <= 0 ) {
		throw CSimException(GetName().c_str(), "State of charge goes below zero.");
	}
	_consumption += (-current_eff * _mBank) * GetOpenCircuitVoltage() * timeElapsed;
	_ccv = PortVoltage(time, _portCurrent);
}

double CLiIonBattery::NextTimeStep(double time, int precision) const {
	double current_eff;
	double difference = 0.01 / (1 + precision); //report SoC difference
	double current = _portCurrent;
	if (abs(current/_mBank) > _capacity) {
		if (current/_mBank > 0) {
			current_eff = pow((abs(current/_mBank/_capacity)),_peukertCharge) * _capacity;
		} else {
			current_eff = -pow((abs(current/_mBank/_capacity)),_peukertDischarge) * _capacity;
		}
	} else {
		current_eff = current/_mBank;
	}
	double step = difference * _capacity * 3600 / abs(current_eff);
	return step;
}

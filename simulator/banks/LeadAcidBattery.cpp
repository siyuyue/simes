// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <cmath>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "LeadAcidBattery.h"
#include "SimException.h"

CLeadAcidBattery::CLeadAcidBattery(void) {
	_stateOfCharge = 1;
	_capacity = 3.4;		// Capacity of each cell in Ah
	_mBank = 1;
	_nBank = 1;
	// Lead acid Battery parameters
	_peukertCharge = 1./1.2;
	_peukertDischarge = 1.15;

	_ageFactor = 1;

	double cali_d = 4;
	double cali_c = 0.25;

	_b11 = -0.6691, _b12 = -13.3547, _b13 = 11.3142, _b14 = 1.5668, _b15 = -0.1627;
	_b21 = 1.9141 * cali_d,  _b22 = -34.8039, _b23 = 0.4076 * cali_d;
	_b31 = 2.0111 * cali_c,  _b32 = -44.7093, _b33 = 1.6628 * cali_c,  _b34 = 1.5973 * cali_c, _b35 = 0.6903 * cali_c, _b36 = 2.5629 * cali_c, _b37 = -44.5850;

	// Add properties
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

CLeadAcidBattery::~CLeadAcidBattery(void) {

}

double CLeadAcidBattery::GetOpenCircuitVoltage() const {
	return _nBank * (_b11 * exp(_b12*_stateOfCharge) + _b13 + _b14*_stateOfCharge + _b15*_stateOfCharge*_stateOfCharge) * _ageFactor;
}

double CLeadAcidBattery::GetStateofCharge() const
{
	return _stateOfCharge;
}

double CLeadAcidBattery::PortVoltage(double time, double current) const
{
	if( current > 0 )			// Charging
	{
		double r = _nBank * (_b31*exp(_b32*_stateOfCharge) + _b33 + _b34*_stateOfCharge + _b35*_stateOfCharge*_stateOfCharge + _b36*exp(_b37*(1-_stateOfCharge)));
		return GetOpenCircuitVoltage() + current/_mBank*r;
	}
	else						// Discharging
	{
		double r = _nBank * (_b21*exp(_b22*_stateOfCharge) + _b23);
		return GetOpenCircuitVoltage() + current/_mBank*r;
	}
}

double CLeadAcidBattery::MaxOutPortCurrent(double time) const
{
	double r = _nBank * (_b21*exp(_b22*_stateOfCharge) + _b23);
	return GetOpenCircuitVoltage()/r*_mBank;
}
void CLeadAcidBattery::Reset()
{
	_stateOfCharge = 1;
	_consumption = 0;
}

double CLeadAcidBattery::NextTimeStep(double time, int precision) const
{
	double current_eff;
	double difference = 0.01 / (1+precision); //report SoC difference
	double current = _portCurrent;
	if (abs(current/_mBank) > _capacity/20)
		if (current/_mBank > 0)
			current_eff = pow((abs(current/_mBank/(_capacity/20))),_peukertCharge) * _capacity/20;
		else
			current_eff = -pow((abs(current/_mBank/(_capacity/20))),_peukertDischarge) * _capacity/20;
	else
		current_eff = current/_mBank;
	return difference * _capacity * 3600 / abs(current_eff);
}

void CLeadAcidBattery::TimeElapse(double time, double timeElapsed)
{
	double current_eff;
	double current = _portCurrent;
	if (abs(current/_mBank) > _capacity/20)
		if (current/_mBank > 0)
			current_eff = pow((abs(current/_mBank/(_capacity/20))),_peukertCharge) * _capacity/20;
		else
			current_eff = -pow((abs(current/_mBank/(_capacity/20))),_peukertDischarge) * _capacity/20;
	else
		current_eff = current/_mBank;
	_stateOfCharge += (current_eff)*timeElapsed/(_capacity*3600);
	if( _stateOfCharge > 1 )
	{
		throw CSimException(GetName().c_str(), "State of charge goes above capacity limit.");
	}
	if( _stateOfCharge <= 0 )
	{
		throw CSimException(GetName().c_str(), "State of charge goes below zero.");
	}
	_consumption += (-current_eff * _mBank) * GetOpenCircuitVoltage() * timeElapsed;
	_ccv = PortVoltage(time, _portCurrent);
}

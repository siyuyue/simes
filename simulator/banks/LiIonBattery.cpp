// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <cmath>
#include "LiIonBattery.h"
#include "SimException.h"

CLiIonBattery::CLiIonBattery(void)
{
	_stateOfCharge = 1;
	_capacity = 2.6;
	_mBank = 1;
	_nBank = 1;
	// Li-ion Battery parameters
	_peukertCharge = 1./1.1;
	_peukertDischarge = 1.2;

	_b11 = -0.67;   _b12 = -16.21;  _b13 = -0.03;
	_b14 = 1.28;    _b15 =  -0.40;  _b16 = 7.55;
	_b21 = 0.05;    _b22 = -4.32;   _b23 = 0.17;
}

CLiIonBattery::~CLiIonBattery(void)
{
}

void CLiIonBattery::Reset()
{
	_stateOfCharge = 1;
	_consumption = 0;
}

double CLiIonBattery::GetStateofCharge() const
{
	return _stateOfCharge;
}

double CLiIonBattery::GetConsumption() const
{
	return _consumption;
}

double CLiIonBattery::GetOpenCircuitVoltage() const
{
	return _nBank * ( _b11*exp(_b12*_stateOfCharge)+_b13*_stateOfCharge*_stateOfCharge*_stateOfCharge+_b14*_stateOfCharge*_stateOfCharge+_b15*_stateOfCharge+_b16 ) / 2;
}

double CLiIonBattery::MaxOutPortCurrent(double time) const
{
    double Rs;
	Rs = (_b21*exp(_b22*_stateOfCharge)+_b23 ) / 2;
    return ( GetOpenCircuitVoltage() ) * _mBank / Rs;
}


double CLiIonBattery::PortVoltage(double time, double current) const
{
    double Rs;
	Rs = (_b21*exp(_b22*_stateOfCharge)+_b23 ) / 2;
    return _nBank * ( GetOpenCircuitVoltage()/_nBank + Rs*current/_mBank );
}

void CLiIonBattery::TimeElapse(double time, double timeElapsed)
{	//Update the charge of Cts and Ctl
	//positive current: discharge
	//Don't use it unless the discharge actually happens
	double current = _portCurrent;
	double current_eff;
    double Rs;
	Rs = (_b21*exp(_b22*_stateOfCharge)+_b23 ) / 2;
	if (abs(current/_mBank) > _capacity)
		if (current/_mBank > 0)
			current_eff = pow((abs(current/_mBank/_capacity)),_peukertCharge) * _capacity;
		else
			current_eff = -pow((abs(current/_mBank/_capacity)),_peukertDischarge) * _capacity;
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
	_consumption += (-current_eff) * GetOpenCircuitVoltage() * timeElapsed;
	_ccv = PortVoltage(time, _portCurrent);
}

double CLiIonBattery::NextTimeStep(double time, int precision) const
{
	double current_eff;
	double difference = 0.01 / (1+precision); //report SoC difference
	double current = _portCurrent;
	if (abs(current/_mBank) > _capacity)
		if (current/_mBank > 0)
			current_eff = pow((abs(current/_mBank/_capacity)),_peukertCharge) * _capacity;
		else
			current_eff = -pow((abs(current/_mBank/_capacity)),_peukertDischarge) * _capacity;
	else
		current_eff = current/_mBank;
	double step = difference * _capacity * 3600 / abs(current_eff);
	return step;
}

bool CLiIonBattery::SetProperty(const string &name, const string& value)
{
	if( name == string("capacity") )
	{
		_capacity = FromString<double>(value);
		return true;
	}
	if( name == string("m_bank") )
	{
		_mBank = FromString<int>(value);
		return true;
	}
	if( name == string("n_bank") )
	{
		_nBank = FromString<int>(value);
		return true;
	}
	if( name == string("state_of_charge") )
	{
		_stateOfCharge = FromString<double>(value);
        return true;
	}
	return false;
}

string CLiIonBattery::GetProperty(const string &name) const
{
	if( name == string("state_of_charge") )
	{
		return ToString<double>(_stateOfCharge);
	}
	return string();
}

bool CLiIonBattery::SetSensor(const string &name, CSensor &sensor)
{
	if( name == string("state_of_charge") )
	{
		sensor.SetPointer(&_stateOfCharge);
		return true;
	}
	if( name == string("closed_circuit_voltage") )
	{
		sensor.SetPointer(&_ccv);
		return true;
	}
    if( name == string("current") )
    {
        sensor.SetPointer(&_portCurrent);
        return true;
    }
	if( name == string("consumption") )
	{
		sensor.SetPointer(&_consumption);
		return true;
	}
	return false;
}
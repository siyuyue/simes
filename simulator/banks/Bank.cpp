// **********************************************
// Bank.cpp
// Implementation of all bank classes
// **********************************************
#include "Bank.h"
#include <iostream>
#include <cmath>

using namespace std;

// **********************************************
// CBankBase
// **********************************************
CBankBase::CBankBase(void) : CPort(true, true, BANK)
{
}

CBankBase::~CBankBase(void)
{
}

// **********************************************
// CSimpleBattery
// **********************************************
CSimpleBattery::CSimpleBattery(void)
{
    // Default values, use SetProperty interface to modify them
	_capacity = 1;
	_openCircuitVoltage = 5;
	_internalResistance = 0;
}

CSimpleBattery::~CSimpleBattery()
{
}

void CSimpleBattery::Reset()
{
	_stateOfCharge = 1.;
}

double CSimpleBattery::GetStateofCharge() const
{
	return _stateOfCharge;
}

double CSimpleBattery::GetOpenCircuitVoltage() const
{
	return _openCircuitVoltage;
}

double CSimpleBattery::Charge(double duration, double current)
{
	double chargeChange = current*duration;
	_stateOfCharge += chargeChange / (3600*_capacity);
	if( _stateOfCharge > 1 )
	{
		// ---- ToDo: Maybe a warning here ----
		_stateOfCharge = 1;
	}
	return chargeChange*_openCircuitVoltage;
}

double CSimpleBattery::Discharge(double duration, double current)
{
	double chargeChange = current*duration;
	_stateOfCharge -= chargeChange / (3600*_capacity);
	if( _stateOfCharge <= 0 )
	{
		// ---- ToDo: Maybe a warning here or reset battery ----
		_stateOfCharge = 0;
	}
	return chargeChange*_openCircuitVoltage;
}

double CSimpleBattery::NextTimeStep(double time, int precision) const
{
	if(abs(_portCurrent) <= EPS)
		return INF;
	else if(_portCurrent < 0)
		return _stateOfCharge*3600*_capacity/(-_portCurrent);
	else
		return (1-_stateOfCharge)*3600*_capacity/(_portCurrent);
}

double CSimpleBattery::PortVoltage(double time, double current) const
{
	return _openCircuitVoltage - current*_internalResistance;
}

double CSimpleBattery::MaxOutPortCurrent(double time) const
{
	if( _internalResistance == 0 )
	{
		return INF;
	}
	return _openCircuitVoltage / _internalResistance;
}

void CSimpleBattery::TimeElapse(double time, double timeElapsed)
{
	if( _portCurrent < 0 )
	{
		Discharge(timeElapsed,-_portCurrent);
	}
	else if( _portCurrent > 0 )
	{
		Charge(timeElapsed,_portCurrent);
	}
}

bool CSimpleBattery::SetProperty(const string &name, const string& value)
{
	if( name == string("capacity") )
	{
		_capacity = FromString<double>(value);
		return true;
	}
	if( name == string("open_circuit_voltage") )
	{
		_openCircuitVoltage = FromString<double>(value);
		return true;
	}
	if( name == string("internal_resistance") )
	{
		_internalResistance = FromString<double>(value);
		return true;
	}
	return false;
}

string CSimpleBattery::GetProperty(const string &name) const
{
	return string();
}

bool CSimpleBattery::SetSensor(const string &name, CSensor &sensor)
{
	if( name == string("state_of_charge") )
	{
		sensor.SetPointer(&_stateOfCharge);
		return true;
	}
    if( name == string("current") )
    {
        sensor.SetPointer(&_portCurrent);
        return true;
    }
	return false;
}

// **********************************************
// CLiionBattery
// **********************************************
CLiIonBattery::CLiIonBattery(void)
{
	_stateOfCharge = 1;
	//_capacity = 0.35;		//total capacity of the bank in Ah
	_capacity = 2.6;
	_mBank = 1;
	_nBank = 1;
	// Li-ion Battery parameters
	_peukertCharge = 1./1.1;
	_peukertDischarge = 1.2;

    // Data is measured from 2 Li-ion batteries in series, so divide V, R and C by 2
	double cali_r = 0.5;

	_b11 = -0.67;   _b12 = -16.21;  _b13 = -0.03;
	_b14 = 1.28;    _b15 =  -0.40;  _b16 = 7.55;
	_b21 = 0.10 * cali_r;    _b22 = -4.32;   _b23 = 0.34 * cali_r;
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

// **********************************************
// CLeadAcidBattery Class
// **********************************************
CLeadAcidBattery::CLeadAcidBattery(void)
{
	_stateOfCharge = 1;
	_capacity = 3.4;		//total capacity of the bank in Ah
	_mBank = 1;
	_nBank = 1;
	// Li-ion Battery parameters
	_peukertCharge = 1./1.2;
	_peukertDischarge = 1.15;

	_ageFactor = 1;

	double cali_d = 4;
	double cali_c = 0.25;

	_b11 = -0.6691, _b12 = -13.3547, _b13 = 11.3142, _b14 = 1.5668, _b15 = -0.1627;
	_b21 = 1.9141 * cali_d,  _b22 = -34.8039, _b23 = 0.4076 * cali_d;
	_b31 = 2.0111 * cali_c,  _b32 = -44.7093, _b33 = 1.6628 * cali_c,  _b34 = 1.5973 * cali_c, _b35 = 0.6903 * cali_c, _b36 = 2.5629 * cali_c, _b37 = -44.5850;
}

CLeadAcidBattery::~CLeadAcidBattery(void)
{

}

double CLeadAcidBattery::GetOpenCircuitVoltage() const
{
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
	_ccv = PortVoltage(time, _portCurrent);
}

bool CLeadAcidBattery::SetProperty(const string &name, const string& value)
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

string CLeadAcidBattery::GetProperty(const string &name) const
{
	return string();
}

bool CLeadAcidBattery::SetSensor(const string &name, CSensor &sensor)
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
	return false;
}

// **********************************************
// CSuperCap Class
// **********************************************
CSuperCap::CSuperCap(void)
{
	_capacitance = 1.;
	_openCircuitVoltage = 1.;
	_selfDischargeRate = 0;
	_seriesResistace = 0;
	_minVoltage = 1.;
    _maxVoltage = 50.;
	_maxPower = 100;
}

CSuperCap::~CSuperCap(void)
{

}

double CSuperCap::GetCapacitance()
{
	return _capacitance;
}

double CSuperCap::GetStateofCharge() const
{
	return _openCircuitVoltage / _maxVoltage;
}

double CSuperCap::PortVoltage(double time, double current) const
{
	return _openCircuitVoltage + current * _seriesResistace;
}

double CSuperCap::MaxOutPortCurrent(double time) const
{
	if( _openCircuitVoltage < _minVoltage )
	{
		return 0;
	}
	if( _seriesResistace == 0 )
	{
		return _maxPower / _openCircuitVoltage;
	}
	return _openCircuitVoltage / _seriesResistace;
}

void CSuperCap::Reset()
{
	;
}

double CSuperCap::NextTimeStep(double time, int precision) const
{
	if( _portCurrent == 0 )
	{

		if( _selfDischargeRate == 0 )
		{
			return INF;
		}
		else
		{
			return 1 / _selfDischargeRate;
		}
	}
	double difference = 0.1/(1+precision);
    return difference * _capacitance / abs(_portCurrent);
}

void CSuperCap::TimeElapse(double time, double timeElapsed)
{
	double energyNow = _capacitance * _openCircuitVoltage * _openCircuitVoltage / 2;
	double energyChanged = _portCurrent * _openCircuitVoltage * timeElapsed;
	double selfDischargeRatio = _openCircuitVoltage < _minVoltage ? 1 : exp(- timeElapsed*_selfDischargeRate);
	double energyNew = energyNow*selfDischargeRatio + energyChanged;
	_openCircuitVoltage = sqrt( energyNew * 2 / _capacitance );
	if( _openCircuitVoltage < _minVoltage )
	{
		// ToDo: Maybe a warning here.
	}
	else if( _openCircuitVoltage > _maxVoltage)
	{
		// ToDo: Maybe a warning here.
	}
}

bool CSuperCap::SetProperty(const string &name, const string& value)
{
	if( name == string("voltage") )
	{
		_openCircuitVoltage = FromString<double>(value);
		return true;
	}
	if( name == string("capacitance") )
	{
		_capacitance = FromString<double>(value);
		return true;
	}
	if( name == string("series_resistance") )
	{
		_seriesResistace = FromString<double>(value);
		return true;
	}
	return false;
}

string CSuperCap::GetProperty(const string &name) const
{
	return string();
}

bool CSuperCap::SetSensor(const string &name, CSensor &sensor)
{
	if( name == string("voltage") )
	{
		sensor.SetPointer(&_openCircuitVoltage);
		return true;
	}
	if( name == string("current") )
	{
		sensor.SetPointer(&_portCurrent);
		return true;
	}
	return false;
}

// **********************************************
// CGrid Class:
// **********************************************
CGrid::CGrid(void)
{
	_voltage = 110;
}

CGrid::~CGrid(void)
{

}

double CGrid::GetStateofCharge() const
{
	return 1;
}

double CGrid::PortVoltage(double time, double current) const
{
	return _voltage;
}

double CGrid::MaxOutPortCurrent(double time) const
{
	return 5;
}

void CGrid::Reset()
{

}

double CGrid::NextTimeStep(double time, int precision) const
{
	return INF;
}

void CGrid::TimeElapse(double time, double timeElapsed)
{

}

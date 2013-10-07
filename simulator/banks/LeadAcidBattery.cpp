#include <cmath>
#include "LeadAcidBattery.h"

CLeadAcidBattery::CLeadAcidBattery(void)
{
	_stateOfCharge = 1;
	_capacity = 3.4;		//total capacity of the bank in Ah
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
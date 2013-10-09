// **********************************************
// CTI.cpp
// Implementation of CCTIPort and CCTI classes
// **********************************************
#include "CTI.h"
#include "SimException.h"
#include <cmath>
#include <iostream>

using namespace std;

CCTIPort::CCTIPort(int id, double *pVoltage) : CPort(true, true, CTI)
{
	_CTIID = id;
	_pCTIVoltage = pVoltage;
	_maxPower = 100;
}

CCTIPort::~CCTIPort()
{
}

double CCTIPort::PortVoltage(double time, double current) const
{
	return *_pCTIVoltage;
}

double CCTIPort::MaxOutPortCurrent(double time) const
{
	return _maxPower / *_pCTIVoltage;
}

CCTI::CCTI(void)
{
	_myID = -1;
	_pVoltageRegulator = NULL;
	_capacitance = 1;
}


CCTI::~CCTI(void)
{
	for(vector<CCTIPort*>::iterator it = _pPorts.begin(); it != _pPorts.end(); it++)
	{
		delete *it;
	}
}

void CCTI::SetID(int myId)
{
	_myID = myId;
}

void CCTI::SetTargetVoltage(double voltage)
{
	_targetVoltage = voltage;
}

void CCTI::Reset()
{
	;
}

void CCTI::FindRegulatorCurrent(double time)
{
	if( _targetVoltage - _voltage > EPS )			// Increase current of the voltage regulator to raise voltage
	{
		if( _pVoltageRegulator->CanPortBeOutput(_isVoltageRegulatorPortA) )
		{
			_pVoltageRegulator->SetPortInputCurrent(time, !_isVoltageRegulatorPortA, _pVoltageRegulator->GetMaxInputPowerCurrent(time, _isVoltageRegulatorPortA));
		}
		else
		{
			_pVoltageRegulator->SetPortOutputCurrent(time, !_isVoltageRegulatorPortA, 0);
		}
	}
	else if( _targetVoltage - _voltage < -EPS )		// Decrease current of the voltage regulator to lower voltage
	{
		if( _pVoltageRegulator->CanPortBeOutput(!_isVoltageRegulatorPortA) )
		{
			_pVoltageRegulator->SetPortInputCurrent(time, _isVoltageRegulatorPortA, _pVoltageRegulator->GetMaxInputPowerCurrent(time, !_isVoltageRegulatorPortA));
		}
		else
		{
			_pVoltageRegulator->SetPortOutputCurrent(time, _isVoltageRegulatorPortA, 0);
		}
	}
	else											// Maintain current balance on the CTI
	{
		double ctiCurrent = 0;
		for(vector<CCTIPort*>::iterator it = _pPorts.begin(); it != _pPorts.end(); it ++)
		{
			if( (*it)->GetConverter() != _pVoltageRegulator )
			{
				ctiCurrent += (*it)->GetPortCurrent();
			}
		}
		if( ctiCurrent > EPS )
		{
			if( _pVoltageRegulator->CanPortBeOutput(!_isVoltageRegulatorPortA) )
			{
				_pVoltageRegulator->SetPortInputCurrent(time, _isVoltageRegulatorPortA, ctiCurrent);
			}
			else
			{
				_pVoltageRegulator->SetPortOutputCurrent(time, _isVoltageRegulatorPortA, 0);
			}
		}
		else if( ctiCurrent < -EPS )
		{
			if( _pVoltageRegulator->CanPortBeOutput(_isVoltageRegulatorPortA) )
			{
				_pVoltageRegulator->SetPortOutputCurrent(time, _isVoltageRegulatorPortA, -ctiCurrent);
			}
			else
			{
				_pVoltageRegulator->SetPortInputCurrent(time, _isVoltageRegulatorPortA, 0);
			}
		}
		else
		{
			if( _pVoltageRegulator->CanPortBeOutput(_isVoltageRegulatorPortA) )
			{
				_pVoltageRegulator->SetPortOutputCurrent(time, _isVoltageRegulatorPortA, 0);
			}
			else
			{
				_pVoltageRegulator->SetPortInputCurrent(time, !_isVoltageRegulatorPortA, 0);
			}
		}
	}
	_pVoltageRegulator->Solve(time);
}

bool CCTI::SetVoltageRegulator(CConverterBase *pRegulator)
{
	for(vector<CCTIPort*>::iterator it = _pPorts.begin(); it != _pPorts.end(); it++ )
	{
		if( (*it)->GetConverter() == pRegulator )
		{
			_pVoltageRegulator = pRegulator;
			_isVoltageRegulatorPortA = (*it)->IsPortA();
			return true;
		}
	}
	return false;
}

double CCTI::GetVoltage() const
{
	return _voltage;
}

double CCTI::GetTargetVoltage() const
{
	return _targetVoltage;
}

double CCTI::GetCapacitance() const
{
	return _capacitance;
}

CPort * CCTI::NewPort()
{
	for(vector<CCTIPort*>::iterator it = _pPorts.begin(); it != _pPorts.end(); it ++)
	{
		if( (*it)->IsFloating() )
		{
			return *it;
		}
	}
	CCTIPort * pPort = new CCTIPort(_myID, &_voltage);
	_pPorts.push_back(pPort);
	return pPort;
}

CPort * CCTI::GetPort(int id) const
{
    if(id < 0 || _pPorts.size() <= size_t(id))
	{
		// Error : id out of range
        throw CSimException(GetName().c_str(), "CTI Port id out of range.");
	}
	return _pPorts[id];
}

double CCTI::NextTimeStep(double time, int precision) const
{
    double difference = 0.1/(1+precision);
	double ctiCurrent = 0;
    for(vector<CCTIPort*>::const_iterator it = _pPorts.begin(); it != _pPorts.end(); it ++)
	{
		ctiCurrent += (*it)->GetPortCurrent();
	}
	double timeStep = INF;
	if( abs(_voltage - _targetVoltage) > EPS )
	{
        timeStep = (_targetVoltage - _voltage) * _capacitance / ctiCurrent ;
		if( timeStep < 0 )
		{
			timeStep = INF;
		}
	}
	if( ctiCurrent != 0 )
	{
        timeStep = min(timeStep, abs(difference * _capacitance/ctiCurrent));
	}
	return timeStep;
}

void CCTI::TimeElapse(double time, double timeElapsed)
{
	// ---- Charge pr discharge CTI ----
	double ctiCurrent = 0;
    for(vector<CCTIPort*>::const_iterator it = _pPorts.begin(); it != _pPorts.end(); it ++)
	{
		ctiCurrent += (*it)->GetPortCurrent();
	}
	double energy = _capacitance * _voltage * _voltage / 2;
	energy += ctiCurrent * _voltage * timeElapsed;
	if( energy < 0)
	{
        throw CSimException(GetName().c_str(), "CTI energy must be non-negative.");
	}
	_voltage = sqrt(2 * energy / _capacitance);
}

CCTI* CCTI::Create()
{
	return new CCTI();
}

void CCTI::CheckIntegrity() const
{
    if( _pVoltageRegulator == NULL )
    {
        throw CSimException(GetName().c_str(), "Voltage regulator is not set.");
    }
    return;
}

bool CCTI::SetProperty(const string &name, const string &value)
{
	if( name == string("voltage") )
	{
		double voltage = FromString<double>(value);
		if( voltage > 0 )
		{
			_voltage = voltage;
			return true;
		}
	}
	else if( name == string("target_voltage") )
	{
		double voltage = FromString<double>(value);
		if( voltage > 0 )
		{
			_targetVoltage = voltage;
			return true;
		}
	}
	else if( name == string("capacitance") )
	{
		double capacitance = FromString<double>(value);
		if( capacitance > 0 )
		{
			_capacitance = capacitance;
			return true;
		}
	}
    else if( name == string("voltage_regulator") )
    {
        for(vector<CCTIPort*>::const_iterator it = _pPorts.begin(); it != _pPorts.end(); it ++)
        {
            if( (*it)->GetConverter()->GetName() == value )
            {
                _pVoltageRegulator = (*it)->GetConverter();
                _isVoltageRegulatorPortA = (*it)->IsPortA();
            }
        }
    }
	return false;
}

string CCTI::GetProperty(const string &name) const
{
	return string();
}

bool CCTI::SetSensor(const string &name, CSensor &sensor)
{
	if( name == string("voltage") )
	{
		sensor.SetPointer(&_voltage);
		return true;
	}
	return false;
}

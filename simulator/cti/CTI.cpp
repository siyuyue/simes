// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <cmath>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "core/SimException.h"
#include "cti/CTI.h"

using namespace std;

CCTIPort::CCTIPort(int id, double *pVoltage) : CPort(true, true, CTI) {
	_CTIID = id;
	_pCTIVoltage = pVoltage;
	_maxPower = 100;
}

CCTIPort::~CCTIPort() {
}

double CCTIPort::PortVoltage(double time, double current) const {
	return *_pCTIVoltage;
}

double CCTIPort::MaxOutPortCurrent(double time) const {
	return _maxPower / *_pCTIVoltage;
}

CCTI::CCTI(void) {
	_myID = -1;
	_pVoltageRegulator = NULL;
	_capacitance = 1;

	// Add properties
	_AddProperty(new CProperty("capacitance", "Capacitance.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_capacitance)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_capacitance), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("voltage", "Terminal voltage.",
		boost::bind(CheckSetter<double>, _1, boost::ref(_voltage), (boost::lambda::_1 > 0)),
		boost::bind(SimpleGetter<double>, _1, boost::ref(_voltage))));
	_AddProperty(new CProperty("target_voltage", "Target terminal voltage.",
		boost::bind(CheckSetter<double>, _1, boost::ref(_targetVoltage), (boost::lambda::_1 > 0)),
		boost::bind(SimpleGetter<double>, _1, boost::ref(_targetVoltage))));
	_AddProperty(new CProperty("voltage_regulator", "Voltage regulator.",
		boost::bind(&CCTI::SetVoltageRegulatorByName, this, _1),
		boost::bind(&CCTI::GetVoltageRegulatorName, this, _1)));
}


CCTI::~CCTI(void) {
	for(vector<CCTIPort*>::iterator it = _pPorts.begin(); it != _pPorts.end(); it++) {
		delete *it;
	}
}

void CCTI::SetID(int myId) {
	_myID = myId;
}

void CCTI::SetTargetVoltage(double voltage) {
	_targetVoltage = voltage;
}

void CCTI::Reset() {
}

void CCTI::FindRegulatorCurrent(double time) {
	if (_targetVoltage - _voltage > EPS) {			// Increase current of the voltage regulator to raise voltage
		if (_pVoltageRegulator->CanPortBeOutput(_isVoltageRegulatorPortA)) {
			_pVoltageRegulator->SetPortInputCurrent(time, !_isVoltageRegulatorPortA, _pVoltageRegulator->GetMaxInputPowerCurrent(time, _isVoltageRegulatorPortA));
		} else {
			_pVoltageRegulator->SetPortOutputCurrent(time, !_isVoltageRegulatorPortA, 0);
		}
	} else if (_targetVoltage - _voltage < -EPS) {		// Decrease current of the voltage regulator to lower voltage
		if (_pVoltageRegulator->CanPortBeOutput(!_isVoltageRegulatorPortA)) {
			_pVoltageRegulator->SetPortInputCurrent(time, _isVoltageRegulatorPortA, _pVoltageRegulator->GetMaxInputPowerCurrent(time, !_isVoltageRegulatorPortA));
		} else {
			_pVoltageRegulator->SetPortOutputCurrent(time, _isVoltageRegulatorPortA, 0);
		}
	} else {											// Maintain current balance on the CTI
		double ctiCurrent = 0;
		for (vector<CCTIPort*>::iterator it = _pPorts.begin(); it != _pPorts.end(); it ++) {
			if ((*it)->GetConverter() != _pVoltageRegulator) {
				ctiCurrent += (*it)->GetPortCurrent();
			}
		}
		if (ctiCurrent > EPS) {
			if (_pVoltageRegulator->CanPortBeOutput(!_isVoltageRegulatorPortA)) {
				_pVoltageRegulator->SetPortInputCurrent(time, _isVoltageRegulatorPortA, ctiCurrent);
			} else {
				_pVoltageRegulator->SetPortOutputCurrent(time, _isVoltageRegulatorPortA, 0);
			}
		} else if (ctiCurrent < -EPS) {
			if (_pVoltageRegulator->CanPortBeOutput(_isVoltageRegulatorPortA)) {
				_pVoltageRegulator->SetPortOutputCurrent(time, _isVoltageRegulatorPortA, -ctiCurrent);
			} else {
				_pVoltageRegulator->SetPortInputCurrent(time, _isVoltageRegulatorPortA, 0);
			}
		} else {
			if (_pVoltageRegulator->CanPortBeOutput(_isVoltageRegulatorPortA)) {
				_pVoltageRegulator->SetPortOutputCurrent(time, _isVoltageRegulatorPortA, 0);
			} else {
				_pVoltageRegulator->SetPortInputCurrent(time, !_isVoltageRegulatorPortA, 0);
			}
		}
	}
	_pVoltageRegulator->Solve(time);
}

bool CCTI::SetVoltageRegulator(CConverterBase *pRegulator) {
	for (vector<CCTIPort*>::const_iterator it = _pPorts.begin(); it != _pPorts.end(); it ++) {
		if ((*it)->GetConverter() == pRegulator) {
			_pVoltageRegulator = pRegulator;
			_isVoltageRegulatorPortA = (*it)->IsPortA();
			return true;
		}
	}
	return false;
}

bool CCTI::SetVoltageRegulatorByName(const string &s) {
	for (vector<CCTIPort*>::const_iterator it = _pPorts.begin(); it != _pPorts.end(); it ++) {
        if ((*it)->GetConverter()->GetName() == s) {
            _pVoltageRegulator = (*it)->GetConverter();
            _isVoltageRegulatorPortA = (*it)->IsPortA();
            return true;
        }
    }
	return false;
}

bool CCTI::GetVoltageRegulatorName(string &s) {
	if (_pVoltageRegulator == NULL) {
		return false;
	}
	s = _pVoltageRegulator->GetName();
	return true;
}

double CCTI::GetVoltage() const {
	return _voltage;
}

double CCTI::GetTargetVoltage() const {
	return _targetVoltage;
}

double CCTI::GetCapacitance() const {
	return _capacitance;
}

CPort *CCTI::NewPort() {
	for (vector<CCTIPort*>::iterator it = _pPorts.begin(); it != _pPorts.end(); it ++) {
		if ((*it)->IsFloating()) {
			return *it;
		}
	}
	CCTIPort *pPort = new CCTIPort(_myID, &_voltage);
	_pPorts.push_back(pPort);
	return pPort;
}

CPort *CCTI::GetPort(int id) const {
    if(id < 0 || _pPorts.size() <= size_t(id)) {
		// Error : id out of range
        throw CSimException(GetName().c_str(), "CTI Port id out of range.");
	}
	return _pPorts[id];
}

double CCTI::NextTimeStep(double time, int precision) const {
    double difference = 0.1/(1+precision);
	double ctiCurrent = 0;
    for (vector<CCTIPort*>::const_iterator it = _pPorts.begin(); it != _pPorts.end(); it ++) {
		ctiCurrent += (*it)->GetPortCurrent();
	}
	double timeStep = INF;
	if (abs(_voltage - _targetVoltage) > EPS) {
        timeStep = (_targetVoltage - _voltage) * _capacitance / ctiCurrent ;
		if (timeStep < 0) {
			timeStep = INF;
		}
	}
	if (ctiCurrent != 0) {
        timeStep = min(timeStep, abs(difference * _capacitance/ctiCurrent));
	}
	return timeStep;
}

void CCTI::TimeElapse(double time, double timeElapsed) {
	// ---- Charge pr discharge CTI ----
	double ctiCurrent = 0;
    for (vector<CCTIPort*>::const_iterator it = _pPorts.begin(); it != _pPorts.end(); it ++) {
		ctiCurrent += (*it)->GetPortCurrent();
	}
	double energy = _capacitance * _voltage * _voltage / 2;
	energy += ctiCurrent * _voltage * timeElapsed;
	if (energy < 0) {
        throw CSimException(GetName().c_str(), "CTI energy must be non-negative.");
	}
	_voltage = sqrt(2 * energy / _capacitance);
}

bool CCTI::CheckIntegrity() const {
    if (_pVoltageRegulator == NULL) {
        throw CSimException(GetName().c_str(), "Voltage regulator is not set.");
    }
    return true;
}

CCTI* CCTI::Create() {
	return new CCTI();
}

void CCTI::Initialize() {
}
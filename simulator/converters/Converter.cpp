// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <cmath>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "core/SimException.h"
#include "converters/Converter.h"
#include "converters/SimpleConverter.h"
#include "converters/DCDCConverter.h"
#include "converters/LUTConverter.h"

using namespace std;

map<string, boost::function<CConverterBase*()> > CConverterBase::factories;

CConverterBase::CConverterBase(void) {
	_pPortA = NULL;
	_pPortB = NULL;
	_outputCurrent = 0;
	_inputCurrent = 0;
	_maxIter = 10000;

	// Add properties
	_AddProperty(new CProperty("input_current", "Input current.",
		boost::bind(SimpleGetter<double>, _1, boost::ref(_inputCurrent))));
	_AddProperty(new CProperty("output_current", "Output current.",
		boost::bind(SimpleGetter<double>, _1, boost::ref(_outputCurrent))));
}

CConverterBase::~CConverterBase(void) {
}

void CConverterBase::SetPortA(CPort *pPort) {
	_pPortA = pPort;
	_pPortA->SetConverter(this, true);
}

void CConverterBase::SetPortB(CPort *pPort) {
	_pPortB = pPort;
	_pPortB->SetConverter(this, false);
}

bool CConverterBase::CanPortBeOutput(bool isPortA) const {
	if (isPortA) {
		return _pPortA->IsInPort() && _pPortB->IsOutPort();
	} else {
		return _pPortB->IsInPort() && _pPortA->IsOutPort();
	}
}

void CConverterBase::SetPortOutputCurrent(double time, bool isPortA, double current) {
	if (isPortA) {
		if (!_pPortA->IsInPort() || !_pPortB->IsOutPort()) {
			// Error here, port A cannot be output.
            throw CSimException(GetName().c_str(),"Port A cannot be output!");
		}
		_isPortAOutput = true;
	} else {
		if (!_pPortB->IsInPort() || !_pPortA->IsOutPort()) {
			// Error here, port B cannot be output.
            throw CSimException(GetName().c_str(),"Port B cannot be output!");
		}
		_isPortAOutput = false;
	}
	_outputCurrent = current;
	_isOutputSet = true;
}

void CConverterBase::SetPortInputCurrent(double time, bool isPortA, double current) {
	if (isPortA) {
		if (!_pPortB->IsInPort() || !_pPortA->IsOutPort()) {
			// Error here, port A cannot be input.
            throw CSimException(GetName().c_str(),"Port A cannot be input!");
		}
		_isPortAOutput = false;
	} else {
		if (!_pPortA->IsInPort() || !_pPortB->IsOutPort()) {
			// Error here, port B cannot be input.
            throw CSimException(GetName().c_str(),"Port B cannot be input!");
		}
		_isPortAOutput = true;
	}
	_inputCurrent = current;
	_isOutputSet = false;
}

void CConverterBase::Solve(double time) {
	if( _isOutputSet ) {
		double outputVoltage;
		CPort * pInputPort;
		CPort * pOutputPort;
		if(_isPortAOutput) {
			outputVoltage = _pPortA->PortVoltage(time, _outputCurrent);
			pOutputPort = _pPortA;
			pInputPort = _pPortB;
		} else {
			outputVoltage = _pPortB->PortVoltage(time, _outputCurrent);
			pOutputPort = _pPortB;
			pInputPort = _pPortA;
		}
		// Solve by iteration
		int count = 0;
        double inputCurrentPre = _inputCurrent + 100;
		while(count < _maxIter && abs(_inputCurrent - inputCurrentPre) > EPS) {
			inputCurrentPre = _inputCurrent;
			_inputCurrent = FindInputCurrent(time, !_isPortAOutput, pInputPort->PortVoltage(time, -_inputCurrent), outputVoltage, _outputCurrent);
			count ++;
		}
		if (count == _maxIter) {
            throw CSimException(GetName().c_str(),"Max number of iterations exceeded!");
		}
		pInputPort->SetPortCurrent(-_inputCurrent);
		pOutputPort->SetPortCurrent(_outputCurrent);
	} else {
		double inputVoltage;
		CPort * pInputPort;
		CPort * pOutputPort;
		if (_isPortAOutput) {
			inputVoltage = _pPortB->PortVoltage(time, _inputCurrent);
			pOutputPort = _pPortA;
			pInputPort = _pPortB;
		} else {
			inputVoltage = _pPortA->PortVoltage(time, _inputCurrent);
			pOutputPort = _pPortB;
			pInputPort = _pPortA;
		}
		// Solve by iteration
		int count = 0;
        double outputCurrentPre = _outputCurrent + 100;
		while (count < _maxIter && abs(_outputCurrent - outputCurrentPre) > EPS) {
			outputCurrentPre = _outputCurrent;
			_outputCurrent = FindOutputCurrent(time, _isPortAOutput, inputVoltage, pOutputPort->PortVoltage(time, _outputCurrent), _inputCurrent);
			count ++;
		}
		if (count == _maxIter) {
            throw CSimException(GetName().c_str(),"Max number of iterations exceeded!");
		}
		pInputPort->SetPortCurrent(-_inputCurrent);
		pOutputPort->SetPortCurrent(_outputCurrent);
	}
}

double CConverterBase::GetMaxInputPowerCurrent(double time, bool isPortA) const {
	CPort * pOutPort;
	CPort * pInPort;
	if (isPortA) {
		pInPort = _pPortA;
		pOutPort = _pPortB;
	} else {
		pInPort = _pPortB;
		pOutPort = _pPortA;
	}
	if (!pOutPort->IsOutPort()) {
		return 0;
	}
	// Assume the output power of the port is unimodal, use tenary search to find the maximum
	double xl = 0;
	double xr = pOutPort->MaxOutPortCurrent(time);
	double xm1, xm2;
	double fl = 0;
	double fr = xr * pOutPort->PortVoltage(time, -xr);
	double fm1, fm2;
	while (xr - xl > EPS) {
		xm1 = (2*xl + xr ) / 3.;
		xm2 = (xl + 2*xr ) / 3.;
		fm1 = xm1 * pOutPort->PortVoltage(time, -xm1);
		fm2 = xm2 * pOutPort->PortVoltage(time, -xm2);
		if (fm1 > fm2) {
			xr = xm2;
			fr = fm2;
		} else {
			xl = xm1;
			fl = fm1;
		}
	}
	return xl;
}

CConverterBase* CConverterBase::Create(const string &derivedType) {
	if (factories.find(derivedType) == factories.end()) {
        return NULL;
    }
    return factories[derivedType](); 
}


void CConverterBase::Initialize() {
	factories["SimpleConverter"] = boost::factory<CSimpleConverter*>();
	factories["DCDCConverter"] = boost::factory<CDCDCConverter*>();
	factories["LUTConverter"] = boost::factory<CLUTConverter*>();
}
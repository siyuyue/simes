// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "core/Port.h"
#include "converters/Converter.h"

CPort::CPort(bool inMode, bool outMode, PortType type) {
	_inPortMode = inMode;
	_outPortMode = outMode;
	_type = type;
	_portCurrent = 0;
	_pConverter = NULL;
}

CPort::~CPort(void) {
}

PortType CPort::GetType() const {
	return _type;
}

bool CPort::IsInPort() const {
	return _inPortMode;
}

bool CPort::IsOutPort() const {
	return _outPortMode;
}

bool CPort::IsFloating() const {
	if(_pConverter == NULL) {
		return true;
	}
	return false;
}

bool CPort::IsPortA() const {
	return _isPortA;
}

void CPort::SetConverter(CConverterBase *pConverter, bool isPortA) {
	_pConverter = pConverter;
	_isPortA = isPortA;
}

CConverterBase *CPort::GetConverter() const {
	return _pConverter;
}

void CPort::SetPortCurrent(double current) {
	_portCurrent = current;
}

double CPort::GetPortCurrent() const {
	return _portCurrent;
}

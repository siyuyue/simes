// **********************************************
// Component.cpp
// Implementation of CSensor class
// **********************************************
#include "Sensor.h"


CSensor::CSensor(const string &target, const string &prop) : _target(target), _property(prop)
{
	_pValue = NULL;
}


CSensor::~CSensor(void)
{
}

double CSensor::Value()
{
	return *_pValue;
}

void CSensor::SetPointer(double *pValue)
{
	_pValue = pValue;
}

bool CSensor::IsPointerSet()
{
	if( _pValue == NULL)
	{
		return false;
	}
	return true;
}

string CSensor::GetTargetPropertyString()
{
	return _target + string(".") + _property;
}

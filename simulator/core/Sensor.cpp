// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "core/Sensor.h"

CSensor::CSensor(const string &target, const string &propertyName, CProperty *property) : _target(target), _propertyName(propertyName) {
	_property = property;
}

CSensor::~CSensor(void) {
}

string CSensor::GetPropertyValue() const {
    string s;
    _property->Get(s);
    return s;
}

string CSensor::GetTargetPropertyString() const {
	return _target + string(".") + _propertyName;
}

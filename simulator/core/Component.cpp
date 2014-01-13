// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "Component.h"

CComponent::CComponent(void) : _name("unnamed"), _pSimulator(NULL)
{
}

CComponent::~CComponent(void)
{
}

void CComponent::_AddProperty(CProperty *property) {
    if (_properties.find(property->GetKey()) != _properties.end()) {
        // A property with the same key already exist!
        return;
    }
    _properties[property->GetKey()] = property;
}

void CComponent::SetName(const std::string &name) {
	_name = name;
}

string CComponent::GetName() const {
	return _name;
}

CSimulator* CComponent::GetSimulator() const {
    return _pSimulator;
}

void CComponent::SetSimulator(CSimulator *pSimulator) {
    _pSimulator = pSimulator;
}

bool CComponent::CheckIntegrity() const {
    return true;
}

// Property interface
bool CComponent::SetProperty(const string &propertyName, const string &value) {
    if (_properties.find(propertyName) == _properties.end()) {
        return false;    
    }
	return _properties[propertyName]->Set(value);
}

bool CComponent::SetPropertyInitial(const string &propertyName, const string &value) {
    if (_properties.find(propertyName) == _properties.end()) {
        return false;    
    }
    return _properties[propertyName]->SetInitial(value);
}

bool CComponent::GetProperty(const string &propertyName, string &value) {
    if (_properties.find(propertyName) == _properties.end()) {
        return false;    
    }
    return _properties[propertyName]->Get(value);
}

CProperty* CComponent::SetSensor(const string &propertyName) {
	if (_properties.find(propertyName) == _properties.end()) {
        return NULL;    
    }
    return _properties[propertyName];
}

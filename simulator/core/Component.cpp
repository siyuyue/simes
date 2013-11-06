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

void CComponent::SetName(const std::string &name)
{
	_name = name;
}

string CComponent::GetName() const
{
	return _name;
}

CSimulator* CComponent::GetSimulator() const
{
    return _pSimulator;
}

void CComponent::SetSimulator(CSimulator *pSimulator)
{
    _pSimulator = pSimulator;
}

// Property interface
void CComponent::CheckIntegrity() const
{
    return;
}

bool CComponent::SetProperty(const std::string &name, const std::string &value)
{
	return false;
}

string CComponent::GetProperty(const std::string &name) const
{
	return string();
}

// Output interface
bool CComponent::SetSensor(const std::string &name, CSensor &sensor)
{
	return false;
}

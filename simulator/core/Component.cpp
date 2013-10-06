// **********************************************
// Component.cpp
// Implementation of CComponent class
// **********************************************
#include "Component.h"

CComponent::CComponent(void) : _name("unnamed"), _pSimulator(NULL)
{
}

CComponent::~CComponent(void)
{
}

void CComponent::SetName(const string &name)
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

bool CComponent::SetProperty(const string &name, const string &value)
{
	return false;
}

string CComponent::GetProperty(const string &name) const
{
	return string();
}

// Output interface
bool CComponent::SetSensor(const string &name, CSensor &sensor)
{
	return false;
}

// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <string>
#include "sources/Source.h"
#include "sources/IdealVoltageSource.h"
#include "sources/ProfileSource.h"

using namespace std;

map<string, boost::function<CSourceBase*()> > CSourceBase::factories;

CSourceBase::CSourceBase(void) : CPort(false, true, SOURCE) {
}

CSourceBase::~CSourceBase(void) {
}

CSourceBase* CSourceBase::Create(const string &derivedType) {
    if (factories.find(derivedType) == factories.end()) {
        return NULL;
    }
    return factories[derivedType](); 
}

void CSourceBase::Initialize() {
    factories["IdealVoltageSource"] = boost::factory<CIdealVoltageSource*>();
    factories["ProfileVoltageSource"] = boost::factory<CProfileSource*>();
}

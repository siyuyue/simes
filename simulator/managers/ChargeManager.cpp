// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <string>
#include "managers/ChargeManager.h"
#include "managers/SimpleManager.h"
#include "managers/ProfileManager.h"
#include "managers/PulseManager.h"

using namespace std;

map<string, boost::function<CChargeManagerBase*()> > CChargeManagerBase::factories;

CChargeManagerBase::CChargeManagerBase(void) {
}

CChargeManagerBase::~CChargeManagerBase(void) {
}


CChargeManagerBase *CChargeManagerBase::Create(const string &derivedType) {
    if (factories.find(derivedType) == factories.end()) {
        return NULL;
    }
    return factories[derivedType]();
}

void CChargeManagerBase::Initialize() {
    factories["SimpleManager"] = boost::factory<CSimpleManager*>();
    factories["ProfileManager"] = boost::factory<CProfileManager*>();
    factories["PulseManager"] = boost::factory<CPulseManager*>();
}

// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <string>

#include "loads/Load.h"
#include "loads/ConstantLoad.h"
#include "loads/ProfileLoad.h"

using namespace std;

map<string, boost::function<CLoadBase*()> > CLoadBase::factories;

CLoadBase::CLoadBase(void) : CPort(true,false,LOAD) {
}

CLoadBase::~CLoadBase(void) {
}

double CLoadBase::PortDefaultVoltage(double time) const {
	return PortVoltage(time, PortDefaultCurrent(time));
}

CLoadBase* CLoadBase::Create(const string &derivedType) {
    if (factories.find(derivedType) == factories.end()) {
        return NULL;
    }
    return factories[derivedType]();
}

void CLoadBase::Initialize() {
    factories["ConstantLoad"] = boost::factory<CConstantLoad*>();
    factories["ProfileLoad"] = boost::factory<CProfileLoad*>();
}

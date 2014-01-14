// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <string>
#include "banks/Bank.h"
#include "banks/SimpleBattery.h"
#include "banks/LiIonBattery.h"
#include "banks/LeadAcidBattery.h"
#include "banks/SuperCap.h"
#include "banks/Grid.h"

using namespace std;

map<string, boost::function<CBankBase*()> > CBankBase::factories;

CBankBase::CBankBase(void) : CPort(true, true, BANK) {
}

CBankBase::~CBankBase(void) {
}

CBankBase * CBankBase::Create(const string &derivedType) {
    if (factories.find(derivedType) == factories.end()) {
        return NULL;
    }
    return factories[derivedType]();
}

void CBankBase::Initialize() {
    factories["SimpleBattery"] = boost::factory<CSimpleBattery*>();
    factories["LiIonBattery"] = boost::factory<CLiIonBattery*>();
    factories["LeadAcidBattery"] = boost::factory<CLeadAcidBattery*>();
    factories["SuperCapacitor"] = boost::factory<CSuperCap*>();
    factories["Grid"] = boost::factory<CGrid*>();
}

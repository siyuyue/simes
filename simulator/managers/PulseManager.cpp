// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "banks/SuperCap.h"
#include "managers/PulseManager.h"

CPulseManager::CPulseManager(void) : _superCapBankName("SC") {
    _period = INF;
    _nextDecisionTime = 0;
    _superCapEnergyTargetHigh = 80;
    _superCapEnergyTargetLow = 10;
    _periodHigh = 10;
    _periodLow = 20;
    _nextPeriodTime = _periodLow;
    _mode = false;

    _AddProperty(new CProperty("period", "Decision period.",
        boost::bind(CheckSetter<double>, _1, boost::ref(_period), (boost::lambda::_1 > 0)),
        boost::bind(SimpleGetter<double>, _1, boost::ref(_period))));
    _AddProperty(new CProperty("supercap_bank_name", "Supercapacitor bank name.",
        boost::bind(SimpleSetter<string>, _1, boost::ref(_superCapBankName)),
        boost::bind(SimpleGetter<string>, _1, boost::ref(_superCapBankName))));
    _AddProperty(new CProperty("supercap_energy_target_high", "Target high supercapacitor energy.",
        boost::bind(CheckSetter<double>, _1, boost::ref(_superCapEnergyTargetHigh), (boost::lambda::_1 > 0)),
        boost::bind(SimpleGetter<double>, _1, boost::ref(_superCapEnergyTargetHigh))));
    _AddProperty(new CProperty("supercap_energy_target_low", "Target low supercapacitor energy.",
        boost::bind(CheckSetter<double>, _1, boost::ref(_superCapEnergyTargetLow), (boost::lambda::_1 > 0)),
        boost::bind(SimpleGetter<double>, _1, boost::ref(_superCapEnergyTargetLow))));
    _AddProperty(new CProperty("period_high", "Duration of high power demand.",
        boost::bind(CheckSetter<double>, _1, boost::ref(_periodHigh), (boost::lambda::_1 > 0)),
        boost::bind(SimpleGetter<double>, _1, boost::ref(_periodHigh))));
    _AddProperty(new CProperty("period_low", "Duration of low power demand.",
        boost::bind(CheckSetter<double>, _1, boost::ref(_periodLow), (boost::lambda::_1 > 0)),
        boost::bind(SimpleGetter<double>, _1, boost::ref(_periodLow))));
}

CPulseManager::~CPulseManager(void) {
}

bool CPulseManager::IsDecisionEpoch(double time) const {
    if (time >= _nextDecisionTime) {
        return true;
    }
    return false;
}

void CPulseManager::Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs) {
    _nextDecisionTime += _period;
    if (time >= _nextPeriodTime) {
        if (_mode) {
            _nextPeriodTime += _periodLow;
        } else {
            _nextPeriodTime += _periodHigh;
        }
        _mode = !_mode;
    }
    if (_mode) {
        for (vector<CBankBase*>::iterator it = pBanks.begin(); it != pBanks.end(); it++) {
            if ((*it)->GetName() == _superCapBankName) {
                CSuperCap *pSuperCap = dynamic_cast<CSuperCap *>(*it);
                if (pSuperCap == NULL) {
                    return;
                }
                double openCircuitVotlage = pSuperCap->PortVoltage(time, 0);
                double currentEnergy = 0.5 * pSuperCap->GetCapacitance() * openCircuitVotlage * openCircuitVotlage;
                pSuperCap->GetConverter()->SetPortOutputCurrent(time, !pSuperCap->IsPortA(), (currentEnergy - _superCapEnergyTargetLow) / pCTIs[0]->GetVoltage() / (_nextPeriodTime - time) );
                break;
            }
        }
    } else {
        for (vector<CBankBase*>::iterator it = pBanks.begin(); it != pBanks.end(); it++) {
            if ((*it)->GetName() == _superCapBankName) {
                CSuperCap *pSuperCap = dynamic_cast<CSuperCap *>(*it);
                if (pSuperCap == NULL) {
                    return;
                }
                double openCircuitVotlage = pSuperCap->PortVoltage(time, 0);
                double currentEnergy = 0.5 * pSuperCap->GetCapacitance() * openCircuitVotlage * openCircuitVotlage;
                pSuperCap->GetConverter()->SetPortInputCurrent(time, !pSuperCap->IsPortA(), (_superCapEnergyTargetHigh - currentEnergy ) / pCTIs[0]->GetVoltage() / (_nextPeriodTime - time) );
                break;
            }
        }
    }
}

void CPulseManager::Reset() {
    _nextDecisionTime = 0;
}

double CPulseManager::NextTimeStep(double time, int precision) const {
    if (_nextDecisionTime - time > 0) {
        return _nextDecisionTime - time;
    } else {
        return 0;
    }
}

void CPulseManager::TimeElapse(double time, double timeElapsed) {
}

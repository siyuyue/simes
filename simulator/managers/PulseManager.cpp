// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "PulseManager.h"
#include "SuperCap.h"

CPulseManager::CPulseManager(void) : _superCapBankName("SC")
{
    _period = INF;
    _nextDecisionTime = 0;
    _superCapEnergyTargetHigh = 80;
    _superCapEnergyTargetLow = 10;
    _periodHi = 10;
    _periodLow = 20;
    _nextPeriodTime = _periodLow;
    _mode = false;
}

CPulseManager::~CPulseManager(void)
{
}

bool CPulseManager::IsDecisionEpoch(double time) const
{
    if(time >= _nextDecisionTime)
    {
        return true;
    }
    return false;
}

void CPulseManager::Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs)
{
    _nextDecisionTime += _period;
    if (time >= _nextPeriodTime) {
        if (_mode) {
            _nextPeriodTime += _periodLow;
        } else {
            _nextPeriodTime += _periodHi;
        }
        _mode = !_mode;
    }
    if (_mode) {
        for (vector<CBankBase*>::iterator it = pBanks.begin(); it != pBanks.end(); it++ ) {
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
        for (vector<CBankBase*>::iterator it = pBanks.begin(); it != pBanks.end(); it++ ) {
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

void CPulseManager::Reset()
{
    _nextDecisionTime = 0;
}

double CPulseManager::NextTimeStep(double time, int precision) const
{
    if(_nextDecisionTime - time > 0)
    {
        return _nextDecisionTime - time;
    }
    else
    {
        return 0;
    }
}

void CPulseManager::TimeElapse(double time, double timeElapsed)
{
}

bool CPulseManager::SetProperty(const string &name, const string &value)
{
    if( name == string("period") )
    {
        _period = FromString<double>(value);
        return true;
    }
    return false;
}
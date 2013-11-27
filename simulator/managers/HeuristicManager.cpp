// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "HeuristicManager.h"
#include "ProfileSource.h"
#include "LiIonBattery.h"
#include "SuperCap.h"

CHeuristicManager::CHeuristicManager(void) : _batteryBankName("Liion"), _superCapBankName("SC")
{
    _period = INF;
    _nextDecisionTime = 0;
    _type = 1;          // type of heuristic: 1-5
    _powerThresholdDischarge = 20000;
    _powerThresholdCharge = 0;
    _maxCurrentDischarge = _powerThresholdDischarge / 375 * 0.95;
    _maxCurrentCharge = _powerThresholdCharge / 375 * 0.95;
    _scVoltageMin = 200;
    _scVoltageMax = 400;
}

CHeuristicManager::~CHeuristicManager(void)
{
}

bool CHeuristicManager::IsDecisionEpoch(double time) const
{
    if(time >= _nextDecisionTime)
    {
        return true;
    }
    return false;
}

void CHeuristicManager::Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs)
{
    _nextDecisionTime += _period;
    CProfileSource *pRegen = static_cast<CProfileSource*>(pSources[0]);
    CLiIonBattery *pLiIon;
    CSuperCap *pSC;
    for(vector<CBankBase*>::iterator it = pBanks.begin(); it != pBanks.end(); it++ ) {
        if ((*it)->GetName() == _batteryBankName) {
            pLiIon = static_cast<CLiIonBattery *>(*it);
        } else if ((*it)->GetName() == _superCapBankName) {
            pSC = static_cast<CSuperCap *>(*it);
        }
    }
    double loadPower = pLoads[0]->PortDefaultVoltage(time) * pLoads[0]->PortDefaultCurrent(time);
    double regenPower = pRegen->MaxOutPortCurrent(time) * pRegen->PortVoltage(time, pRegen->MaxOutPortCurrent(time));

    switch (_type) {
    case 1:
        pRegen->GetConverter()->SetPortInputCurrent(time, pRegen->IsPortA(), pRegen->MaxOutPortCurrent(time));
        if (loadPower > 0 && loadPower <= _powerThresholdDischarge) {
            pSC->GetConverter()->SetPortOutputCurrent(time, pRegen->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pLiIon->GetConverter());
        } else if (regenPower > 0 && regenPower <= _powerThresholdCharge && pLiIon->GetStateofCharge() < 0.9999) {
            pSC->GetConverter()->SetPortOutputCurrent(time, pRegen->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pLiIon->GetConverter());
        } else if (loadPower > 0 && pSC->PortVoltage(time, 0) < _scVoltageMin) {
            pSC->GetConverter()->SetPortOutputCurrent(time, pRegen->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pLiIon->GetConverter());
        } else if (regenPower > 0 && pSC->PortVoltage(time, 0) > _scVoltageMax) {
            pSC->GetConverter()->SetPortOutputCurrent(time, pRegen->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pLiIon->GetConverter());
        } else if (loadPower > 0) {
            pLiIon->GetConverter()->SetPortOutputCurrent(time, !pLiIon->IsPortA(), _maxCurrentDischarge);
            pCTIs[0]->SetVoltageRegulator(pSC->GetConverter());
        } else if (regenPower > 0) {
            pLiIon->GetConverter()->SetPortInputCurrent(time, !pLiIon->IsPortA(), _maxCurrentCharge);
            pCTIs[0]->SetVoltageRegulator(pSC->GetConverter());
        } else {
            pLiIon->GetConverter()->SetPortOutputCurrent(time, !pLiIon->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pSC->GetConverter());
        }
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    }
}

void CHeuristicManager::Reset()
{
    _nextDecisionTime = 0;
}

double CHeuristicManager::NextTimeStep(double time, int precision) const
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

void CHeuristicManager::TimeElapse(double time, double timeElapsed)
{
}

bool CHeuristicManager::SetProperty(const string &name, const string &value)
{
    if( name == string("period") )
    {
        _period = FromString<double>(value);
        return true;
    }
    if( name == string("type") )
    {
        _type = FromString<int>(value);
        return true;
    }
    if( name == string("battery_bank_name") )
    {
        _batteryBankName = value;
        return true;
    }
    if( name == string("supercap_bank_name") )
    {
        _superCapBankName = value;
        return true;
    }
    return false;
}
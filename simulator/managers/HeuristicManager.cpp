// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "HeuristicManager.h"
#include "ProfileSource.h"
#include "LiIonBattery.h"
#include "SuperCap.h"
#include "Simulator.h"
#include <fstream>

CHeuristicManager::CHeuristicManager(void) : _batteryBankName("Liion"), _superCapBankName("SC")
{
    _period = INF;
    _nextDecisionTime = 0;
    _type = 1;          // type of heuristic: 1-5
    _powerThresholdDischarge = 20000;
    _powerThresholdCharge = 10000;
    _maxCurrentDischarge = _powerThresholdDischarge / 375 * 0.95;
    _maxCurrentCharge = _powerThresholdCharge / 375 * 0.95;
    _scVoltageRef = 300;
    _scVoltageMin = 200;
    _scVoltageMax = 400;
    _kp = -0.01;
    _ki = -0.0001;
    _hysterisRefLow = 280;
    _hysterisRefHigh =320;
    _hysterisDischarge = 12000;
    _hysterisCharge = 12000;
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
    pRegen->GetConverter()->SetPortInputCurrent(time, pRegen->IsPortA(), pRegen->MaxOutPortCurrent(time));

    switch (_type) {
    case 1:
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
        if (loadPower > 0 && pSC->PortVoltage(time, 0) > _scVoltageMin) {
            pLiIon->GetConverter()->SetPortOutputCurrent(time, !pLiIon->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pSC->GetConverter());
        } else if (regenPower > 0 && pSC->PortVoltage(time, 0) < _scVoltageMax) {
            pLiIon->GetConverter()->SetPortOutputCurrent(time, !pLiIon->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pSC->GetConverter());
        } else {
            pSC->GetConverter()->SetPortOutputCurrent(time, pRegen->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pLiIon->GetConverter());
        }
        break;

    case 3:
        if (loadPower > 0 && pSC->PortVoltage(time, 0) < _scVoltageMin) {
            pSC->GetConverter()->SetPortOutputCurrent(time, pRegen->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pLiIon->GetConverter());
        } else if (regenPower > 0 && pSC->PortVoltage(time, 0) > _scVoltageMax) {
            pSC->GetConverter()->SetPortOutputCurrent(time, pRegen->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pLiIon->GetConverter());
        } else {
            double err = pSC->PortVoltage(time, 0) - _scVoltageRef;
            _errIntegral += err;
            pLiIon->GetConverter()->SetPortOutputCurrent(time, !pLiIon->IsPortA(), _kp * err + _ki * _errIntegral);
            pCTIs[0]->SetVoltageRegulator(pSC->GetConverter());
        }
        break;

    case 4:
        if (loadPower > 0 && pSC->PortVoltage(time, 0) < _scVoltageMin) {
            pSC->GetConverter()->SetPortOutputCurrent(time, pRegen->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pLiIon->GetConverter());
        } else if (regenPower > 0 && pSC->PortVoltage(time, 0) > _scVoltageMax) {
            pSC->GetConverter()->SetPortOutputCurrent(time, pRegen->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pLiIon->GetConverter());
        } else {
            double err = pSC->PortVoltage(time, 0) - (200 + (150 - _speed[_idx]) * 1);
            _errIntegral += err;
            pLiIon->GetConverter()->SetPortOutputCurrent(time, !pLiIon->IsPortA(), _kp * err + _ki * _errIntegral);
            pCTIs[0]->SetVoltageRegulator(pSC->GetConverter());
        }
        break;

    case 5:
        if (loadPower > 0 && pSC->PortVoltage(time, 0) < _scVoltageMin) {
            pSC->GetConverter()->SetPortOutputCurrent(time, pRegen->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pLiIon->GetConverter());
        } else if (regenPower > 0 && pSC->PortVoltage(time, 0) > _scVoltageMax) {
            pSC->GetConverter()->SetPortOutputCurrent(time, pRegen->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pLiIon->GetConverter());
        } else if (pSC->PortVoltage(time, 0) > _hysterisRefLow && pSC->PortVoltage(time, 0) < _hysterisRefHigh) {
            pLiIon->GetConverter()->SetPortOutputCurrent(time, !pLiIon->IsPortA(), 0);
            pCTIs[0]->SetVoltageRegulator(pSC->GetConverter());
        } else if (pSC->PortVoltage(time, 0) <= _hysterisRefLow) {
            double err = _hysterisRefLow - pSC->PortVoltage(time, 0);
            if (err > 20) {
                err = 20;
            }
            pLiIon->GetConverter()->SetPortOutputCurrent(time, !pLiIon->IsPortA(), err / 20. * _hysterisDischarge / 375);
            pCTIs[0]->SetVoltageRegulator(pSC->GetConverter());
        } else {
            double err = _hysterisRefHigh - pSC->PortVoltage(time, 0);
            if (err > 20) {
                err = 20;
            }
            pLiIon->GetConverter()->SetPortInputCurrent(time, !pLiIon->IsPortA(), err / 20. * _hysterisCharge / 375);
            pCTIs[0]->SetVoltageRegulator(pSC->GetConverter());
        }
        break;
    }
}

void CHeuristicManager::Reset()
{
    _nextDecisionTime = 0;
    _errIntegral = 0;
    _idx = 0;
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
    while( _idx+1 != _time.size() && time + timeElapsed >= _time[_idx + 1] )
    {
        _idx ++;
    }
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
    if( name == string("power_threshold_discharge") )
    {
        _powerThresholdDischarge = FromString<double>(value);
        _maxCurrentDischarge = _powerThresholdDischarge / 375 * 0.95;
        return true;
    }
    if( name == string("power_threshold_charge") )
    {
        _powerThresholdCharge = FromString<double>(value);
        _maxCurrentCharge = _powerThresholdCharge / 375 * 0.95;
        return true;
    }
    if( name == string("speed_profile") )
    {
        ifstream inputProfile(string(GetSimulator()->GetPathPrefix() + value).c_str());
        if(!inputProfile)
        {
            return false;
        }
        _time.clear();
        _speed.clear();
        while(inputProfile)
        {
            double time, current;
            inputProfile >> time >> current;
            _time.push_back(time);
            _speed.push_back(current);
        }
        return true;
    }
    return false;
}
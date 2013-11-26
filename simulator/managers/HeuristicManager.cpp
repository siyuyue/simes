// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "HeuristicManager.h"

CHeuristicManager::CHeuristicManager(void)
{
    _period = INF;
    _nextDecisionTime = 0;
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
    switch (type) {
    case 1:
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
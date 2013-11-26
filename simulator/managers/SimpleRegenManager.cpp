// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "SimpleRegenManager.h"

CSimpleRegenManager::CSimpleRegenManager(void)
{
    _period = INF;
    _nextDecisionTime = 0;
}

CSimpleRegenManager::~CSimpleRegenManager(void)
{
}

bool CSimpleRegenManager::IsDecisionEpoch(double time) const
{
    if(time >= _nextDecisionTime)
    {
        return true;
    }
    return false;
}

void CSimpleRegenManager::Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs)
{
    _nextDecisionTime += _period;
    for (vector<CSourceBase*>::iterator it = pSources.begin(); it != pSources.end(); it++ ) {
        (*it)->GetConverter()->SetPortInputCurrent(time, (*it)->IsPortA(), (*it)->MaxOutPortCurrent(time));
    }
}

void CSimpleRegenManager::Reset()
{
    _nextDecisionTime = 0;
}

double CSimpleRegenManager::NextTimeStep(double time, int precision) const
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

void CSimpleRegenManager::TimeElapse(double time, double timeElapsed)
{
}

bool CSimpleRegenManager::SetProperty(const string &name, const string &value)
{
    if( name == string("period") )
    {
        _period = FromString<double>(value);
        return true;
    }
    return false;
}
// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CSimpleRegenManager class:
// - Inherited from ChargeManagerBase
// - Charge from source
// **********************************************
#include "ChargeManager.h"

class CSimpleRegenManager : public CChargeManagerBase
{
private:
    double _nextDecisionTime;
    double _period;
public:
    CSimpleRegenManager(void);
    virtual ~CSimpleRegenManager(void);
    virtual bool IsDecisionEpoch(double time) const;
    virtual void Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs);
    virtual void Reset();
    virtual double NextTimeStep(double time, int precision) const;
    virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string &value);
};
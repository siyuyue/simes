// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CSimpleManager class:
// - Inherited from ChargeManagerBase
// - Does nothing
// **********************************************
#include "ChargeManager.h"

class CSimpleManager : public CChargeManagerBase
{
private:
	double _nextDecisionTime;
	double _period;
public:
	CSimpleManager(void);
	virtual ~CSimpleManager(void);
    virtual bool IsDecisionEpoch(double time) const;
	virtual void Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs);
	virtual void Reset();
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string &value);
};
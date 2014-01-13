// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CPulseManager class:
// - Inherited from ChargeManagerBase
// - Does nothing
// **********************************************
#include "ChargeManager.h"

class CPulseManager : public CChargeManagerBase {
private:
    double _nextDecisionTime;
    double _period;
    double _superCapEnergyTargetHigh;
    double _superCapEnergyTargetLow;
    double _periodHigh;
    double _periodLow;
    double _nextPeriodTime;
    bool _mode;      // Low = 0, High = 1
    string _superCapBankName;
public:
    CPulseManager(void);
    virtual ~CPulseManager(void);
    virtual bool IsDecisionEpoch(double time) const;
    virtual void Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs);
    virtual void Reset();
    virtual double NextTimeStep(double time, int precision) const;
    virtual void TimeElapse(double time, double timeElapsed);
};
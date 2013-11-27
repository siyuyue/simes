// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CHeuristicManager class:
// - Inherited from ChargeManagerBase
// - Does nothing
// **********************************************
#include "ChargeManager.h"

class CHeuristicManager : public CChargeManagerBase
{
private:
    double _nextDecisionTime;
    double _period;
    string _batteryBankName;
    string _superCapBankName;
    int _type;          // type of heuristic: 1-5
    double _powerThresholdDischarge;
    double _powerThresholdCharge;
    double _maxCurrentDischarge;
    double _maxCurrentCharge;
    double _scVoltageMin;
    double _scVoltageMax;
public:
    CHeuristicManager(void);
    virtual ~CHeuristicManager(void);
    virtual bool IsDecisionEpoch(double time) const;
    virtual void Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs);
    virtual void Reset();
    virtual double NextTimeStep(double time, int precision) const;
    virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string &value);
};
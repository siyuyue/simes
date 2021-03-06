// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CProfileManager class:
// - Inherited from ChargeManagerBase
// - Sets up bank current and cti target voltage based on an input file
// **********************************************
#include <vector>
#include <map>
#include "managers/ChargeManager.h"

class CProfileManager : public CChargeManagerBase {
private:
	int _bankNumber;
	int _ctiNumber;
	int _idx;
	vector<double> _time;
	map<string, vector<double>*> _ctiVoltages;
    map<string, vector<double>*> _bankCurrents;
public:
	CProfileManager(void);
	virtual ~CProfileManager(void);
    virtual bool IsDecisionEpoch(double time) const;
	virtual void Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs);
	virtual void Reset();
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
    virtual bool CheckIntegrity() const;
    bool SetProfile(const string& s);
};

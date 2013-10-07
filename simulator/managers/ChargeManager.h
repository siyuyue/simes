// **********************************************
// ChargeManagerBase class:
// - Inherited from CTiming
// - Abstract class
// **********************************************
#pragma once
#include <vector>
#include "config.h"
#include "Bank.h"
#include "Load.h"
#include "Source.h"
#include "CTI.h"
#include "Component.h"

class CChargeManagerBase : public CComponent
{
public:
	CChargeManagerBase(void);
	virtual ~CChargeManagerBase(void);
    virtual bool IsDecisionEpoch(double time) const = 0;
	virtual void Decision(double time, std::vector<CLoadBase*> pLoads, std::vector<CBankBase*> pBanks, std::vector<CSourceBase*> pSources, std::vector<CConverterBase*> pConverters, std::vector<CCTI*> pCTIs) = 0;
    static CChargeManagerBase* Create(const string &derivedType);
};



// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// ChargeManagerBase class:
// - Inherited from CComponent
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
	virtual void Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs) = 0;
    static CChargeManagerBase* Create(const string &derivedType);
};



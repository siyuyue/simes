// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CChargeManagerBase factory method
// Register all charge manager classes here by their names
// **********************************************
#include <string>
#include "ChargeManager.h"
#include "SimpleManager.h"
#include "ProfileManager.h"

CChargeManagerBase *CChargeManagerBase::Create(const string &derivedType)
{
	if( derivedType == string("SimpleManager") )
	{
		return new CSimpleManager();
	}
	if( derivedType == string("ProfileManager") )
	{
		return new CProfileManager();
	}
	return NULL;
}

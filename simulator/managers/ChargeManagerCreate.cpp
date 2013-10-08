// **********************************************
// ChargeManagerCreate.cpp
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

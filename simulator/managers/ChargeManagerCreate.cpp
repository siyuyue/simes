// **********************************************
// ChargeManagerCreate.cpp
// Register all charge manager classes here by their names
// **********************************************
#include "ChargeManager.h"

using namespace std;

CChargeManagerBase *CChargeManagerBase::Create(const string &derivedType)
{
	if( derivedType == string("SimpleManager") )
	{
		return new CSimpleManager();
	}
	if( derivedType == string("SGManager") )
	{
		return new CSGManager();
	}
    if( derivedType == string("HAManager") )
    {
        return new CHAManager();
    }
	if( derivedType == string("RLManager") )
	{
		return new CRLManager();
	}
	return NULL;
}

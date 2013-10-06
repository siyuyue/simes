// **********************************************
// BankCreate.cpp
// Register all bank classes here by their names
// **********************************************
#include "Bank.h"
#include <string>

using namespace std;

CBankBase * CBankBase::Create(const string &derivedType)
{
	if( derivedType == string("SimpleBattery") )
	{
		return new CSimpleBattery();
	}
	if( derivedType == string("LiionBattery") )
	{
		return new CLiIonBattery();
	}
	if( derivedType == string("LeadAcidBattery") )
	{
		return new CLeadAcidBattery();
	}
	if( derivedType == string("SuperCapacitor") )
	{
		return new CSuperCap();
	}
	if( derivedType == string("Grid") )
	{
		return new CGrid();
	}
	return NULL;
}

// **********************************************
// CBankBase factory method
// Register all bank classes here by their names
// **********************************************
#include "Bank.h"
#include "SimpleBattery.h"
#include "LiIonBattery.h"
#include "LeadAcidBattery.h"
#include "SuperCap.h"
#include "Grid.h"
#include <string>

using namespace std;

CBankBase * CBankBase::Create(const string &derivedType)
{
	if( derivedType == string("SimpleBattery") )
	{
		return new CSimpleBattery();
	}
	if( derivedType == string("LiIonBattery") )
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

// **********************************************
// SourceCreate.cpp
// Register all source classes here by their names
// **********************************************
#include "Source.h"
#include <string>

using namespace std;

CSourceBase* CSourceBase::Create(const string &derivedType)
{
	if( derivedType == string("IdealVoltageSource") )
	{
		return new CIdealVoltageSource();
	}
	return NULL;
}

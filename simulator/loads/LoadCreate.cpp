// **********************************************
// LoadCreate.cpp
// Register all load classes here by their names
// **********************************************
#include "Load.h"
#include <string>

using namespace std;

CLoadBase* CLoadBase::Create(const string &derivedType)
{
	if( derivedType == string("ConstantLoad") )
	{
		return new CConstantLoad();
	}
	if (derivedType == string("ProfileLoad"))
	{
		return new CProfileLoad();
	}
	return NULL;
}

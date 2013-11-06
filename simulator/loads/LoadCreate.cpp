// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CLoadBase factory method
// Register all load classes here by their names
// **********************************************
#include <string>
#include "Load.h"
#include "ConstantLoad.h"
#include "ProfileLoad.h"

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

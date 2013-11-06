// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CSourceBase factory method
// Register all source classes here by their names
// **********************************************
#include <string>
#include "Source.h"
#include "IdealVoltageSource.h"

CSourceBase* CSourceBase::Create(const string &derivedType)
{
	if( derivedType == string("IdealVoltageSource") )
	{
		return new CIdealVoltageSource();
	}
	return NULL;
}

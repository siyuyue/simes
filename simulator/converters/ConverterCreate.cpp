// **********************************************
// ConverterCreate.cpp
// Register all converter classes here by their names
// **********************************************
#include "Converter.h"

using namespace std;

CConverterBase* CConverterBase::Create(const string &derivedType)
{
	if( derivedType == "SimpleConverter" )
	{
		return new CSimpleConverter();
	}
	else if( derivedType == "DCDCConverter" )
	{
		return new CDCDCConverter();
	}
    else if( derivedType == "LUTConverter" )
    {
        return new CLUTConverter();
    }
	return NULL;
}

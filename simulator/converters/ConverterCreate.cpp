// **********************************************
// CConverterBase factory method
// Register all converter classes here by their names
// **********************************************
#include "Converter.h"
#include "SimpleConverter.h"
#include "DCDCConverter.h"
#include "LUTConverter.h"

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

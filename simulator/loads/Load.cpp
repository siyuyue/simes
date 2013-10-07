#include "Load.h"

CLoadBase::CLoadBase(void) : CPort(true,false,LOAD)
{
}

CLoadBase::~CLoadBase(void)
{
}

double CLoadBase::PortDefaultVoltage(double time) const
{
	return PortVoltage(time, PortDefaultCurrent(time));
}

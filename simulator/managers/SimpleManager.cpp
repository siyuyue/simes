#include "SimpleManager.h"

CSimpleManager::CSimpleManager(void)
{
	_period = INF;
	_nextDecisionTime = 0;
}

CSimpleManager::~CSimpleManager(void)
{
}

bool CSimpleManager::IsDecisionEpoch(double time) const
{
	if(time >= _nextDecisionTime)
	{
		return true;
	}
	return false;
}

void CSimpleManager::Decision(double time, std::vector<CLoadBase*> pLoads, std::vector<CBankBase*> pBanks, std::vector<CSourceBase*> pSources, std::vector<CConverterBase*> pConverters, std::vector<CCTI*> pCTIs)
{
	_nextDecisionTime += _period;
}

void CSimpleManager::Reset()
{
	_nextDecisionTime = 0;
}

double CSimpleManager::NextTimeStep(double time, int precision) const
{
	if(_nextDecisionTime - time > 0)
	{
		return _nextDecisionTime - time;
	}
	else
	{
		return 0;
	}
}

void CSimpleManager::TimeElapse(double time, double timeElapsed)
{
}

bool CSimpleManager::SetProperty(const string &name, const string &value)
{
	if( name == string("period") )
	{
		_period = FromString<double>(value);
		return true;
	}
	return false;
}
// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "ProfileManager.h"
#include "Simulator.h"
#include "SimException.h"

CProfileManager::CProfileManager(void)
{
	_bankNumber = 0;
	_ctiNumber = 0;
}

CProfileManager::~CProfileManager(void)
{
    for( map<string, vector<double>*>::iterator it = _bankCurrents.begin(); it != _bankCurrents.end(); it++ )
        delete it->second;
    for( map<string, vector<double>*>::iterator it = _ctiVoltages.begin(); it != _ctiVoltages.end(); it++ )
        delete it->second;
}

bool CProfileManager::IsDecisionEpoch(double time) const
{
	return true;
}

void CProfileManager::Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs)
{
	for(vector<CBankBase*>::iterator it = pBanks.begin(); it != pBanks.end(); it++ )
	{
        if( _bankCurrents.find((*it)->GetName()) == _bankCurrents.end() )
            continue;
        vector<double> *pBankCurrents = _bankCurrents[(*it)->GetName()];
        if( ((*pBankCurrents)[_idx] > 0 && (*it)->GetStateofCharge() > 0.99) || ((*pBankCurrents)[_idx] < 0 && (*it)->GetStateofCharge() < 0.01) )
        {
            (*it)->GetConverter()->SetPortOutputCurrent(time, (*it)->IsPortA(), 0 );
        }
        else
        {
            if((*pBankCurrents)[_idx] > 0)
            {
                (*it)->GetConverter()->SetPortOutputCurrent(time, (*it)->IsPortA(), (*pBankCurrents)[_idx] );
            }
            else
            {
                (*it)->GetConverter()->SetPortInputCurrent(time, (*it)->IsPortA(), -(*pBankCurrents)[_idx] );
            }
        }
	}
	for(vector<CCTI*>::iterator it = pCTIs.begin(); it != pCTIs.end(); it++ )
	{
		if( _ctiVoltages.find((*it)->GetName()) == _ctiVoltages.end() )
            continue;
        vector<double> *pCTIVoltages = _ctiVoltages[(*it)->GetName()];
        (*it)->SetTargetVoltage((*pCTIVoltages)[_idx]);
	}
}

void CProfileManager::Reset()
{
	_idx = 0;
}

double CProfileManager::NextTimeStep(double time, int precision) const
{
	if( _idx+1 != _time.size() )
	{
		return _time[_idx + 1] - time;
	}
	return INF;
}

void CProfileManager::TimeElapse(double time, double timeElapsed)
{
	while( _idx+1 != _time.size() && time + timeElapsed >= _time[_idx + 1] )
	{
		_idx ++;
	}
}

bool CProfileManager::SetProperty(const string &name, const string &value)
{
	if( name == string("input_file") )
	{
        ifstream inputProfile(string(GetSimulator()->GetPathPrefix() + value).c_str());
		if(!inputProfile)
		{
			return false;
		}
		_time.clear();
        for( map<string, vector<double>*>::iterator it = _bankCurrents.begin(); it != _bankCurrents.end(); it++ )
            delete it->second;
        for( map<string, vector<double>*>::iterator it = _ctiVoltages.begin(); it != _ctiVoltages.end(); it++ )
        	delete it->second;
        _bankCurrents.clear();
        _ctiVoltages.clear();

		int number;
		string name;

		inputProfile >> number;
		if( number <= 0 )
		{
			return false;
		}		
		_ctiNumber = number;

		inputProfile >> number;
		if( number <= 0 )
		{
			return false;
		}
		_bankNumber = number;

		_time.clear();
        std::vector<string> bankNames;
        std::vector<string> ctiNames;

        for( int i=0;i<_ctiNumber;i++)
		{
			inputProfile >> name;
            ctiNames.push_back(name);
            _ctiVoltages[name] = new vector<double>();
		}

		for( int i=0;i<_bankNumber;i++)
		{
			inputProfile >> name;
            bankNames.push_back(name);
            _bankCurrents[name] = new vector<double>();
		}

		while(inputProfile)
		{
			double time, voltage, current;
			inputProfile >> time;
			_time.push_back(time);
			for( int i=0;i<_ctiNumber;i++)
			{
				inputProfile >> voltage;
				_ctiVoltages[ctiNames[i]]->push_back(voltage);
			}
			for( int i=0;i<_bankNumber;i++)
			{
				inputProfile >> current;
                _bankCurrents[bankNames[i]]->push_back(current);
			}
		}
		return true;
	}
    return false;
}

void CProfileManager::CheckIntegrity() const
{
    if(_time.empty())
    {
        throw CSimException(GetName().c_str(), "Input profile is not set.");
    }
}

string CProfileManager::GetProperty(const string &name) const
{
	return string();
}
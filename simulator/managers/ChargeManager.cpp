// **********************************************
// ChargeManager.cpp
// Implementation of all charge manager classes
// **********************************************
#include "ChargeManager.h"
#include "SimException.h"
#include "Simulator.h"

// **********************************************
// CChargeManagerBase class
// **********************************************
CChargeManagerBase::CChargeManagerBase(void)
{
}

CChargeManagerBase::~CChargeManagerBase(void)
{
}

// **********************************************
// CSimpleManager class
// **********************************************
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

void CSimpleManager::Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs)
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

// **********************************************
// CSGManager class
// **********************************************
CSGManager::CSGManager(void)
{
	_bankNumber = 0;
}

CSGManager::~CSGManager(void)
{
    for( map<string, vector<double>*>::iterator it = _bankCurrents.begin(); it != _bankCurrents.end(); it++ )
        delete it->second;
}

bool CSGManager::IsDecisionEpoch(double time) const
{
	return true;
}

void CSGManager::Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs)
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
	pCTIs[0]->SetTargetVoltage(_ctiVoltage[_idx]);
}

void CSGManager::Reset()
{
	_idx = 0;
}

double CSGManager::NextTimeStep(double time, int precision) const
{
	if( _idx+1 != _time.size() )
	{
		return _time[_idx + 1] - time;
	}
	return INF;
}

void CSGManager::TimeElapse(double time, double timeElapsed)
{
	while( _idx+1 != _time.size() && time + timeElapsed >= _time[_idx + 1] )
	{
		_idx ++;
	}
}

bool CSGManager::SetProperty(const string &name, const string &value)
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
        _bankCurrents.clear();

		int number;
		string name;
		inputProfile >> number;
		if( number <= 0 )
		{
			return false;
		}

		_bankNumber = number;

		_time.clear();
        vector<string> bankNames;

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
			inputProfile >> voltage;
			_ctiVoltage.push_back(voltage);
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

void CSGManager::CheckIntegrity() const
{
    if(_time.empty())
    {
        throw CSimException(GetName().c_str(), "Input profile is not set.");
    }
}

string CSGManager::GetProperty(const string &name) const
{
	return string();
}

// **********************************************
// CHAManager class
// **********************************************
CHAManager::CHAManager(void)
{
    _offPeakDuration = 14*3600;
    _peakDuration = 10*3600;
}

CHAManager::~CHAManager(void)
{
}

void CHAManager::Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs)
{
    if( time < _offPeakDuration )
    {
        pCTIs[0]->SetVoltageRegulator( pSources[0]->GetConverter() );
        // Charging
        for(vector<CBankBase*>::iterator it = pBanks.begin(); it != pBanks.end(); it++ )
        {
            if( _bankCapacity.find((*it)->GetName()) != _bankCapacity.end() )
                (*it)->GetConverter()->SetPortOutputCurrent(time, (*it)->IsPortA(), _bankCapacity[(*it)->GetName()]*(1-(*it)->GetStateofCharge())/(_offPeakDuration-time)*3600 );
        }

    }
    else if( time < _offPeakDuration + _peakDuration )
    {
        pCTIs[0]->SetVoltageRegulator( pLoads[0]->GetConverter() );
        pSources[0]->GetConverter()->SetPortInputCurrent(time, pSources[0]->IsPortA(), 0 );
        // Discharging
        for(vector<CBankBase*>::iterator it = pBanks.begin(); it != pBanks.end(); it++ )
        {
            if( _bankCapacity.find((*it)->GetName()) != _bankCapacity.end() )
                (*it)->GetConverter()->SetPortInputCurrent(time, (*it)->IsPortA(), _bankCapacity[(*it)->GetName()]*((*it)->GetStateofCharge()-0.01)/(_peakDuration+_offPeakDuration-time)*3600 );
        }
    }
    else
    {
        for(vector<CBankBase*>::iterator it = pBanks.begin(); it != pBanks.end(); it++ )
        {
            (*it)->GetConverter()->SetPortOutputCurrent(time, (*it)->IsPortA(), 0 );
        }
    }
    CSimpleManager::Decision(time, pLoads, pBanks, pSources, pConverters, pCTIs);
}

bool CHAManager::SetProperty(const string &name, const string &value)
{
    if( name == string("input_file") )
    {
        ifstream inputFile(value.c_str());
        if(!inputFile)
        {
            return false;
        }
        int bankNum;
        string bankName;
        double capacity;
        string line;
        getline(inputFile,line);
        istringstream ss(line);
        ss >> bankNum;
        _bankCapacity.clear();
        for(int i=0;i<bankNum;i++)
        {
            string line;
            getline(inputFile,line);
            istringstream ss(line);
            ss >> bankName >> capacity;
            _bankCapacity[bankName] = capacity;
        }
        return true;
    }
    return CSimpleManager::SetProperty(name,value);
}

// **********************************************
// CRLManager class
// **********************************************
CRLManager::CRLManager(void)
{
	_lastState = -1;
	_lastAction = -1;
	_loadPowerStateNum = 3;
	_supercapStateNum = 5;
	_batteryCurrentActionNum = 3;
	_ctiVoltageActionNum = 1;
	_lastTime = 0;
	_lastConsumption = 0;
	_alpha = 0.5;
	_gamma = 0.01;
	_epsilon = 0.1;
	_exploration = true;
	_q = new double[_loadPowerStateNum * _supercapStateNum * _batteryCurrentActionNum * _ctiVoltageActionNum];
	_bankCurrent = new double[_batteryCurrentActionNum];
	_ctiVoltage = new double[_ctiVoltageActionNum];

	_maxLoadPower = 1.5;

	_bankCurrent[0] = 0;
    _bankCurrent[1] = 0.35;
	_bankCurrent[2] = INF;

	_ctiVoltage[0] = 3.3;
}

CRLManager::~CRLManager(void)
{
	delete []_q;
	delete []_bankCurrent;
	delete []_ctiVoltage;
}

bool CRLManager::IsDecisionEpoch(double time) const
{
	return true;
}

void CRLManager::Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs)
{
	double loadPower = 0;
	double superCapSoC;
	CSuperCap *pSupercap;
	CLiIonBattery *pBatteryBank;
	for(vector<CLoadBase*>::iterator it = pLoads.begin(); it != pLoads.end(); it++)
	{
		loadPower += (*it)->PortDefaultVoltage(time) * (*it)->PortDefaultCurrent(time);
	}
	for(vector<CBankBase*>::iterator it = pBanks.begin(); it != pBanks.end(); it++)
	{
		if( (*it)->GetName() == _supercapName )
		{
			pSupercap = static_cast<CSuperCap*>(*it);
			superCapSoC = (*it)->GetStateofCharge();
		}
		if( (*it)->GetName() == _batterName )
		{
			pBatteryBank = static_cast<CLiIonBattery*>(*it);
		}
	}
	
	if( time > _lastTime + 300 )
	{
		double superCapEnergy = pSupercap->GetCapacitance()*pSupercap->PortVoltage(time, 0)*pSupercap->PortVoltage(time, 0) / 2;
		double loadConsumption = static_cast<CProfileLoad*>(pLoads[0])->GetConsumption();
		if((pBatteryBank->GetConsumption() - superCapEnergy - _lastConsumption) != 0)
		{
			double eff = (loadConsumption - _lastLoadConsumption) / (pBatteryBank->GetConsumption() - superCapEnergy - _lastConsumption);
			_rating[_ctiVoltageIndex] = _rating[_ctiVoltageIndex] * (1-_alpha) + eff * _alpha;
		}

		int maxI = 0;
		double maxR = _rating[0];
		for(int i = 1; i<10; i++)
		{
			if( _rating[i] > maxR )
			{
				maxI = i;
				maxR = _rating[i];
			}
		}

		if( _exploration && double(rand())/RAND_MAX < _epsilon )
		{
			maxI = rand() % 10;
		}

		_ctiVoltageIndex = maxI;

		_lastTime = time;
		_lastConsumption = pBatteryBank->GetConsumption() - superCapEnergy;
		_lastLoadConsumption = loadConsumption;
	}
	_expertID = _ctiVoltageIndex;
	
	if( superCapSoC < _socMax[_ctiVoltageIndex] && loadPower < _pth[_ctiVoltageIndex] )
	{
		pCTIs[0]->SetVoltageRegulator(pSupercap->GetConverter());
		pBatteryBank->GetConverter()->SetPortInputCurrent(time, pBatteryBank->IsPortA(), 0.35);
	}
	else if( loadPower > _pth[_ctiVoltageIndex] && superCapSoC > _socMin[_ctiVoltageIndex] )
	{
		pCTIs[0]->SetVoltageRegulator(pSupercap->GetConverter());
		pBatteryBank->GetConverter()->SetPortInputCurrent(time, pBatteryBank->IsPortA(), 0.35);
	}
	else
	{
		pSupercap->GetConverter()->SetPortOutputCurrent(time, pSupercap->IsPortA(), 0);
		pCTIs[0]->SetVoltageRegulator(pBatteryBank->GetConverter());
	}

    // Change battery
    if( pBatteryBank->GetStateofCharge() < 0.01 )
        pBatteryBank->Reset();
	//pCTIs[0]->SetTargetVoltage(_optCTIVoltage[_ctiVoltageIndex]);
}

void CRLManager::Reset()
{
	_lastState = -1;
	_lastAction = -1;
	_lastTime = 0;
	_lastConsumption = 0;
	_lastLoadConsumption = 0;
	_lastSuperCapSoC = 0.1;
	_exploration = true;
	int totalStateNum = _loadPowerStateNum * _supercapStateNum * _batteryCurrentActionNum * _ctiVoltageActionNum;
	for(int i=0; i<totalStateNum; i++)
	{
		_q[i] = 0;
	}

	_socMin[0] = 0.33;
	_socMin[1] = 0.33;
	_socMin[2] = 0.33;
	_socMin[3] = 0.33;
	_socMin[4] = 0.33;
	_socMin[5] = 0.33;
	_socMin[6] = 0.33;
	_socMin[7] = 0.33;
	_socMin[8] = 0.33;
	_socMin[9] = 0.33;

	_socMax[0] = 0.4;
	_socMax[1] = 0.38;
	_socMax[2] = 0.36;
	_socMax[3] = 0.4;
	_socMax[4] = 0.38;
	_socMax[5] = 0.36;
	_socMax[6] = 0.4;
	_socMax[7] = 0.36;
	_socMax[8] = 0.4;
	_socMax[9] = 0.36;

	_pth[0] = 1.;
	_pth[1] = 1.;
	_pth[2] = 1.;
	_pth[3] = 0.9;
	_pth[4] = 0.9;
	_pth[5] = 0.9;
	_pth[6] = 1.1;
	_pth[7] = 1.1;
	_pth[8] = 1.2;
	_pth[9] = 1.2;
	
	for( int i = 0; i<10; i++)
	{
		_rating[i] = 1;
		_optCTIVoltage[i] = 3.3;
	}
	_ctiVoltageIndex = 0;
}

double CRLManager::NextTimeStep(double time, int precision) const
{
	return INF;
}

void CRLManager::TimeElapse(double time, double timeElapsed)
{
	;
}

bool CRLManager::SetProperty(const string &name, const string &value)
{
	if( name == string("exploration") )
	{
		if( value == string("true") )
		{
			_exploration = true;
		}
		else if( value == string("false") )
		{
			_exploration = false;
		}
		return true;
	}
	if( name == string("battery_name") )
	{
		_batterName = value;
		return true;
	}
	if( name == string("supercap_name") )
	{
		_supercapName = value;
		return true;
	}
	return false;
}

int CRLManager::LoadPowerState(double loadPower) const
{
	//int loadPowerState =  loadPower / _maxLoadPower * _loadPowerStateNum;
	//if( loadPowerState >= _loadPowerStateNum )
	//{
	//	loadPowerState = _loadPowerStateNum - 1;
	//}
	//return loadPowerState;
	if( loadPower < 0.1 )
	{
		return 0;
	}
	if( loadPower < 0.3 )
	{
		return 0;
	}
	if( loadPower < 1.6 )
	{
		return 1;
	}
	if( loadPower < 2.2 )
	{
		return 1;
	}
	return 1;
}

int CRLManager::SuperCapState(double supercapSoC) const
{
    int supercapState = int((supercapSoC - 0.33) / 0.07 * _supercapStateNum);
	if( supercapState >= _supercapStateNum )
	{
		supercapState = _supercapStateNum - 1;
	}
	else if( supercapState < 0 )
	{
		supercapState = 0;
	}
	return supercapState;
	/*if( supercapSoC < 0.33 )
	{
		return 0;
	}
	if( supercapSoC < 0.35 )
	{
		return 1;
	}
	if( supercapSoC < 0.37 )
	{
		return 2;
	}
	if( supercapSoC < 0.39 )
	{
		return 3;
	}
	return 4;*/
}

bool CRLManager::SetSensor(const string &name, CSensor &sensor)
{
	if( name == string("expert_id") )
	{
		sensor.SetPointer(&_expertID);
		return true;
	}
	return false;
}

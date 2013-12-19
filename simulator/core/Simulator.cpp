// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "Simulator.h"
#include "SimException.h"
#include <iostream>
#include <sstream>

using namespace std;

CCommand::CCommand()
{
	time = 0;
    type = INVALID;
}

CCommand::~CCommand()
{
}

bool operator < (const CCommand &c1, const CCommand &c2)
{
	return c1.time < c2.time;
}

CCommand CCommand::FromString(const string &commandString)
{
    istringstream ss(commandString);
    string type;
    CCommand command;
    ss >> type;
    if( type == string("get") )
    {
        command.type = CCommand::GET;
        ss >> command.targetName;
        ss >> command.propertyName;
    }
    else if( type == string("set") )
    {
        command.type = CCommand::SET;
        ss >> command.targetName;
        ss >> command.propertyName;
        ss >> command.propertyValue;
    }
    else if( type == string("sim") )
    {
        command.type = CCommand::SIMULATE;
        ss >> command.time;
    }
    else if( type == string("finish") )
    {
        command.type = CCommand::FINISH;
    }
    return command;
}

// **********************************************
// CSimulator
// **********************************************
CSimulator::CSimulator(void)
{
	_pManager = NULL;
	_time = 0;
    _sensorInterval = 0;
	_precisionLevel = 0;
}

CSimulator::~CSimulator(void)
{
	for( vector<CComponent*>::iterator it = _pComponents.begin(); it != _pComponents.end(); it++ )
	{
		delete *it;
	}
	_sensorOutput.close();
}

CPort * CSimulator::_GetPort(const string &name)
{
	if( _usedNames.find(name) == _usedNames.end() )
	{
		// Invalid port name
		return NULL;
	}
	if( _CTIMapping.find(name) != _CTIMapping.end() )
	{
		return _pCTIs[_CTIMapping[name]]->NewPort();
	}
	else if( _loadMapping.find(name) != _loadMapping.end() )
	{
		return _pLoads[_loadMapping[name]];
	}
	else if( _bankMapping.find(name) != _bankMapping.end() )
	{
		return _pBanks[_bankMapping[name]];
	}
	else if( _sourceMapping.find(name) != _sourceMapping.end() )
	{
		return _pSources[_sourceMapping[name]];
	}
	// Port not found
	return NULL;
}

void CSimulator::Reset()
{
	_time = 0;
	for( vector<CComponent*>::iterator it = _pComponents.begin(); it != _pComponents.end(); it++ )
	{
		(*it)->Reset();
	}
	_sensorOutput.close();
	_sensorOutput.clear();
	_sensorOutput.open(string(_projectName+string(".out")).c_str());
	_sensorOutput << "Time";
	for( vector<CSensor *>::iterator it = _pSensors.begin(); it != _pSensors.end(); it++ )
	{
		_sensorOutput << " \t" << (*it)->GetTargetPropertyString();
	}
	_sensorOutput << endl;
    _lastSensorTime = -1e10;
}

void CSimulator::Run(double timeToRun)
{
    try
    {
        while(_time < timeToRun)
        {
            // Satisfy all the load power by default
            for( vector<CLoadBase*>::iterator it = _pLoads.begin(); it != _pLoads.end(); it++ )
            {
                (*it)->GetConverter()->SetPortOutputCurrent(_time, (*it)->IsPortA(), (*it)->PortDefaultCurrent(_time));
            }
            if( _pManager->IsDecisionEpoch(_time) )
            {
                _pManager->Decision(_time,_pLoads,_pBanks,_pSources,_pConverters,_pCTIs);
            }
            for( vector<CConverterBase*>::iterator it = _pConverters.begin(); it != _pConverters.end(); it++ )
            {
                (*it)->Solve(_time);
            }
            for( vector<CCTI*>::iterator it = _pCTIs.begin(); it != _pCTIs.end(); it++ )
            {
                (*it)->FindRegulatorCurrent(_time);
            }
            double timeMin = INF;
            for( vector<CComponent*>::iterator it = _pComponents.begin(); it != _pComponents.end(); it++ )
            {
                timeMin = min(timeMin, (*it)->NextTimeStep(_time, _precisionLevel));
            }
            if( _time + timeMin > timeToRun )
            {
                timeMin = timeToRun - _time;
            }
            for( vector<CComponent*>::iterator it = _pComponents.begin(); it != _pComponents.end(); it++ )
            {
                (*it)->TimeElapse(_time,timeMin);
            }
            _time += timeMin;

            if( _time - _lastSensorTime >= _sensorInterval )
            {
                _sensorOutput << _time;
                for( vector<CSensor *>::iterator it = _pSensors.begin(); it != _pSensors.end(); it++ )
                {
                    _sensorOutput << " \t" << (*it)->Value();
                }
                _sensorOutput << endl;
                _lastSensorTime = _time;
            }
        }
    }
    catch(CSimException &e)
    {
        cout << "[Error at time " << _time << "!]" << e.componentName << ": " << e.exceptionMessage << endl;
        throw CSimException("Simulator","Error during simulation.");
    }
}

void CSimulator::SetProjectName(const string &name)
{
	_projectName = name;
}

void CSimulator::SetPathPrefix(string prefix)
{
    _pathPrefix = prefix;
}

string CSimulator::GetPathPrefix() const
{
    return _pathPrefix;
}

void CSimulator::CheckIntegrity() const
{
    // Check whether the HEES system is properly set
    for( vector<CComponent*>::const_iterator it = _pComponents.begin(); it != _pComponents.end(); it++ )
    {
        (*it)->CheckIntegrity();
    }
}

bool CSimulator::SetManager(const string &name, CChargeManagerBase *pManager)
{
	if( _usedNames.find(name) != _usedNames.end() || pManager == NULL )
	{
		return false;
	}
    pManager->SetSimulator(this);
	_usedNames.insert(name);
	_managerName = name;
	_pManager = pManager;
	_pManager->SetName(name);
	_pComponents.push_back(_pManager);
	return true;
}

bool CSimulator::AddLoad(const string &name, CLoadBase * pLoad)
{
	if( _usedNames.find(name) != _usedNames.end() || pLoad == NULL )
	{
		// Name already used
		return false;
	}
    pLoad->SetSimulator(this);
	_usedNames.insert(name);
	_loadMapping[name] = _pLoads.size();
	_pLoads.push_back(pLoad);
	pLoad->SetName(name);
	_pComponents.push_back(pLoad);
	return true;
}

bool CSimulator::AddSource(const string &name, CSourceBase * pSource)
{
	if( _usedNames.find(name) != _usedNames.end() || pSource == NULL )
	{
		// Name already used
		return false;
	}
    pSource->SetSimulator(this);
	_usedNames.insert(name);
	_sourceMapping[name] = _pSources.size();
	_pSources.push_back(pSource);
	pSource->SetName(name);
	_pComponents.push_back(pSource);
	return true;
}

bool CSimulator::AddBank(const string &name, CBankBase * pBank)
{
	if( _usedNames.find(name) != _usedNames.end() || pBank == NULL )
	{
		// Name already used
		return false;
	}
    pBank->SetSimulator(this);
	_usedNames.insert(name);
	_bankMapping[name] = _pBanks.size();
	_pBanks.push_back(pBank);
	pBank->SetName(name);
	_pComponents.push_back(pBank);
	return true;
}

bool CSimulator::AddCTI(const string &name, CCTI * pCTI)
{
	if( _usedNames.find(name) != _usedNames.end() || pCTI == NULL )
	{
		// Name already used
		return false;
	}
    pCTI->SetSimulator(this);
	pCTI->SetID(_pCTIs.size());
	_usedNames.insert(name);
	_CTIMapping[name] = _pCTIs.size();
	_pCTIs.push_back(pCTI);
	pCTI->SetName(name);
	_pComponents.push_back(pCTI);
	return true;
}

bool CSimulator::AddConverter(const string &name, CConverterBase * pConverter)
{
	if( _usedNames.find(name) != _usedNames.end() || pConverter == NULL )
	{
		// Name already used
		return false;
	}
    pConverter->SetSimulator(this);
	_usedNames.insert(name);
	_converterMapping[name] = _pConverters.size();
	_pConverters.push_back(pConverter);
	pConverter->SetName(name);
	_pComponents.push_back(pConverter);
	return true;
}

bool CSimulator::AddSensor(const string &target, const string &prop)
{
    CComponent *pComponent;
    CSensor *pSensor;
	if( (pComponent = GetComponent(target)) != NULL )
	{
		pSensor = new CSensor(target, prop);
		if( pComponent->SetSensor(prop, *pSensor) )
		{
			_pSensors.push_back(pSensor);
			return true;
		}
	}
	return false;
}

bool CSimulator::SetCTIVoltageRegulator(const string &ctiName, const string &converterName)
{
	CCTI * pCTI = GetCTI(ctiName);
	if( pCTI == NULL )
	{
		return false;
	}
	CConverterBase * pConverter = GetConverter(converterName);
	if( pConverter == NULL )
	{
		return false;
	}
	pCTI->SetVoltageRegulator(pConverter);
	return true;
}

bool CSimulator::SetConverterPorts(const string &converterName, const string &portAName, const string &portBName)
{
    CConverterBase *pConverter = GetConverter(converterName);
	if(pConverter == NULL)
	{
		return false;
	}

	CPort *pPortA = _GetPort(portAName);

	if(pPortA == NULL || !pPortA->IsFloating())
	{
		return false;
	}
	CPort *pPortB = _GetPort(portBName);

	if(pPortB == NULL || !pPortB->IsFloating())
	{
		return false;
	}

	pConverter->SetPortA(pPortA);
	pConverter->SetPortB(pPortB);
	return true;
}

bool CSimulator::SetComponentProperty(const string &compName, const string &propertyName, const string &propertyValue)
{
    CComponent *pComponent = GetComponent(compName);
	if( pComponent == NULL )
	{
		return false;
	}
	return pComponent->SetProperty(propertyName, propertyValue);
}

bool CSimulator::IssueCommand(CCommand &command)
{
	Run(command.time);
	switch(command.type)
	{
	case CCommand::FINISH:
		return true;
	case CCommand::SIMULATE:
		return true;
	case CCommand::SET:
		return SetComponentProperty(command.targetName, command.propertyName, command.propertyValue);
	case CCommand::GET:
		command.propertyValue = GetComponentProperty(command.targetName, command.propertyName);
		return true;
	}
	return false;
}

bool CSimulator::SetSensorInterval(double interval)
{
    if( interval >= _sensorInterval )
    {
        _sensorInterval = interval;
        return true;
    }
    return false;
}

bool CSimulator::SetPrecisionLevel(int level)
{
    if( level >= 0 && level <= 10 )
    {
        _precisionLevel = level;
        return true;
    }
    return false;
}

double CSimulator::GetTime(void)
{
    return _time;
}

string CSimulator::GetComponentProperty(const string &compName, const string &propertyName) const
{
	CComponent * pComponent = GetComponent(compName);
	if( pComponent == NULL )
	{
		return string();
	}
	return pComponent->GetProperty(propertyName);
}

CLoadBase*	CSimulator::GetLoad(const string &name) const
{
	if( _loadMapping.find(name) == _loadMapping.end() )
	{
		return NULL;
	}
    return _pLoads[_loadMapping.at(name)];
}

CSourceBase* CSimulator::GetSource(const string &name) const
{
	if( _sourceMapping.find(name) == _sourceMapping.end() )
	{
		return NULL;
	}
    return _pSources[_sourceMapping.at(name)];
}

CBankBase*	CSimulator::GetBank(const string &name) const
{
	if( _bankMapping.find(name) == _bankMapping.end() )
	{
		return NULL;
	}
    return _pBanks[_bankMapping.at(name)];
}

CCTI* CSimulator::GetCTI(const string &name) const
{
	if( _CTIMapping.find(name) == _CTIMapping.end() )
	{
		return NULL;
	}
    return _pCTIs[_CTIMapping.at(name)];
}

CConverterBase* CSimulator::GetConverter(const string &name) const
{
	if( _converterMapping.find(name) == _converterMapping.end() )
	{
		return NULL;
	}
    return _pConverters[_converterMapping.at(name)];
}

CComponent* CSimulator::GetComponent(const string &name) const
{
	if( _usedNames.find(name) == _usedNames.end() )
	{
		// Invalid Component name
		return NULL;
	}
	if( _CTIMapping.find(name) != _CTIMapping.end() )
	{
        return _pCTIs[_CTIMapping.at(name)];
	}
	else if( _loadMapping.find(name) != _loadMapping.end() )
	{
        return _pLoads[_loadMapping.at(name)];
	}
	else if( _bankMapping.find(name) != _bankMapping.end() )
	{
        return _pBanks[_bankMapping.at(name)];
	}
	else if( _sourceMapping.find(name) != _sourceMapping.end() )
	{
        return _pSources[_sourceMapping.at(name)];
	}
	else if( _converterMapping.find(name) != _converterMapping.end() )
	{
        return _pConverters[_converterMapping.at(name)];
	}
	else if( _managerName == name )
	{
		return _pManager;
	}
	// Component not found
	return NULL;
}

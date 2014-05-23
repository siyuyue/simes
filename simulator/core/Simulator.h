// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#pragma once

#include "banks/Bank.h"
#include "converters/Converter.h"
#include "core/Port.h"
#include "core/config.h"
#include "core/Sensor.h"
#include "cti/CTI.h"
#include "loads/Load.h"
#include "managers/ChargeManager.h"
#include "sources/Source.h"
#include <map>
#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;

// **********************************************
// CCommand
// - Command to CSimulator
// **********************************************
class CCommand {
public:
    enum {SET, GET, SIMULATE, FINISH, INVALID} type;
	double time;
	string targetName;
	string propertyName;
	string propertyValue;
public:
	CCommand();
	~CCommand();
    static CCommand FromString(const string &commandString);
};

bool operator < (const CCommand &c1, const CCommand &c2);

// **********************************************
// CSimulator class:
// - Main class for simulation
// **********************************************
class CSimulator {
private:
    static bool _initialized;
	string _projectName;
    string _pathPrefix;
	set<string> _usedNames;

	string _managerName;
	CChargeManagerBase *_pManager;
	map<string, int> _CTIMapping;
	map<string, int> _loadMapping;
	map<string, int> _bankMapping;
	map<string, int> _sourceMapping;
	map<string, int> _converterMapping;

	vector<CLoadBase*>		_pLoads;
	vector<CBankBase*>		_pBanks;
	vector<CSourceBase*>	_pSources;
	vector<CConverterBase*> _pConverters;
	vector<CCTI*>			_pCTIs;
	vector<CComponent*>		_pComponents;

	vector<CSensor*> _pSensors;

	double _time;
    double _sensorInterval;                     // Minimum interval for outputting results
    double _lastSensorTime;
	int _precisionLevel;						// Simulation precision, 10 - most precise, 0 - fastest

	ofstream _sensorOutput;

    CPort* _GetPort(const string &name);
public:
	CSimulator(void);
	~CSimulator(void);
	void Reset();
	void Run(double time);
    void SetProjectName(const string &name);
    void SetPathPrefix(string prefix);
    string GetPathPrefix() const;
    void CheckIntegrity() const;
    bool SetManager(const string &name, CChargeManagerBase *pManager);
    bool AddLoad(const string &name, CLoadBase * pLoad);
    bool AddSource(const string &name, CSourceBase * pSource);
    bool AddBank(const string &name, CBankBase * pBank);
    bool AddCTI(const string &name, CCTI * pCTI);
    bool AddConverter(const string &name, CConverterBase * pConverter);
    bool AddSensor(const string &target, const string &prop);
    bool SetCTIVoltageRegulator(const string &ctiName, const string &converterName);
    bool SetConverterPorts(const string &converterName, const string &portAName, const string &portBName);
    bool SetComponentProperty(const string &compName, const string &propertyName, const string &propertyValue);
    bool SetComponentPropertyInitial(const string &compName, const string &propertyName, const string &propertyValue);
    bool GetComponentProperty(const string &compName, const string &propertyName, string &propertyValue) const;
    bool IssueCommand(CCommand &command);
    bool SetSensorInterval(double interval);
    bool SetPrecisionLevel(int level);
    double GetTime(void);
    CLoadBase*			GetLoad(const string &name) const;
    CSourceBase*		GetSource(const string &name) const;
    CBankBase*			GetBank(const string &name) const;
    CCTI*				GetCTI(const string &name) const;
    CConverterBase*     GetConverter(const string &name) const;
    CComponent*     	GetComponent(const string &name) const;
    static void Initialize();
};

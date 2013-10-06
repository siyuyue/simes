// **********************************************
// ChargeManager.h
// Definition of all charge manager classes
// **********************************************
#pragma once
#include <string>
#include <map>
#include <cmath>
#include "Bank.h"
#include "Load.h"
#include "Source.h"
#include "CTI.h"
#include "config.h"
#include "Component.h"

using namespace std;

// **********************************************
// ChargeManagerBase class:
// - Inherited from CTiming
// - Abstract class
// **********************************************
class CChargeManagerBase : public CComponent
{
public:
	CChargeManagerBase(void);
	virtual ~CChargeManagerBase(void);
    virtual bool IsDecisionEpoch(double time) const = 0;
	virtual void Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs) = 0;
    static CChargeManagerBase* Create(const string &derivedType);
};

// **********************************************
// CSimpleManager class:
// - Inherited from ChargeManagerBase
// - Basically does nothing
// **********************************************
class CSimpleManager : public CChargeManagerBase
{
private:
	double _nextDecisionTime;
	double _period;
public:
	CSimpleManager(void);
	virtual ~CSimpleManager(void);
    virtual bool IsDecisionEpoch(double time) const;
	virtual void Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs);
	virtual void Reset();
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string &value);
};

// **********************************************
// CSGManager class:
// - Inherited from ChargeManagerBase
// - For validation application
// **********************************************
class CSGManager : public CChargeManagerBase
{
private:
	int _bankNumber;
	int _idx;
	vector<double> _time;
	vector<double> _ctiVoltage;
    map<string, vector<double>*> _bankCurrents;
public:
	CSGManager(void);
	virtual ~CSGManager(void);
    virtual bool IsDecisionEpoch(double time) const;
	virtual void Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs);
	virtual void Reset();
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string& value);
    virtual string GetProperty(const string &name) const;
    virtual void CheckIntegrity() const;
};

// **********************************************
// CHAManager class:
// - Inherited from ChargeManagerBase
// - For home application
// **********************************************
class CHAManager : public CSimpleManager
{
private:
    double _offPeakDuration;
    double _peakDuration;
    map<string,double> _bankCapacity;
public:
    CHAManager(void);
    virtual ~CHAManager(void);
    virtual void Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs);
    virtual bool SetProperty(const string &name, const string& value);
};

// **********************************************
// CRLManager class:
// - Inherited from ChargeManagerBase
// - For mobile application: online RL algorithm
// **********************************************
class CRLManager : public CChargeManagerBase
{
private:
	int _lastState;						// For reinforcement learning
	int _lastAction;
	int _loadPowerStateNum;
	int _supercapStateNum;
	int _batteryCurrentActionNum;
	int _ctiVoltageActionNum;
	double _lastTime;
	double _lastConsumption;
	double _alpha;
	double _gamma;
	double _epsilon;
	bool _exploration;
	double *_q;
	double _maxLoadPower;
	double *_bankCurrent;
	double *_ctiVoltage;

	double _lastSuperCapSoC;
	double _lastLoadConsumption;
	string _batterName;
	string _supercapName;

	double _pth[10];
	double _socMin[10];
	double _socMax[10];
	double _optCTIVoltage[10];
	double _rating[10];
	int _ctiVoltageIndex;
	double _expertID;
public:
	CRLManager(void);
	virtual ~CRLManager(void);
    virtual bool IsDecisionEpoch(double time) const;
	virtual void Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs);
	virtual void Reset();
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string& value);
    virtual bool SetSensor(const string &name, CSensor &sensor);

    int LoadPowerState(double loadPower) const;
    int SuperCapState(double supercapSoC) const;
};

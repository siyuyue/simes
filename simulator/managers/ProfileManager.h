// **********************************************
// CProfileManager class:
// - Inherited from ChargeManagerBase
// - Sets up bank current and cti target voltage based on an input file
// **********************************************
#include <vector>
#include <map>
#include "ChargeManager.h"

class CProfileManager : public CChargeManagerBase
{
private:
	int _bankNumber;
	int _ctiNumber;
	int _idx;
	std::vector<double> _time;
	std::map<string, vector<double>*> _ctiVoltages;
    std::map<string, vector<double>*> _bankCurrents;
public:
	CProfileManager(void);
	virtual ~CProfileManager(void);
    virtual bool IsDecisionEpoch(double time) const;
	virtual void Decision(double time, std::vector<CLoadBase*> pLoads, std::vector<CBankBase*> pBanks, std::vector<CSourceBase*> pSources, std::vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs);
	virtual void Reset();
    virtual double NextTimeStep(double time, int precision) const;
	virtual void TimeElapse(double time, double timeElapsed);
    virtual bool SetProperty(const string &name, const string& value);
    virtual string GetProperty(const string &name) const;
    virtual void CheckIntegrity() const;
};

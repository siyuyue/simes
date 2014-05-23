// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/ref.hpp>
#include "managers/SimpleManager.h"

CSimpleManager::CSimpleManager(void) {
	_period = INF;
	_nextDecisionTime = 0;

	_AddProperty(new CProperty("period", "Decision period.",
        boost::bind(CheckSetter<double>, _1, boost::ref(_period), (boost::lambda::_1 > 0)),
        boost::bind(SimpleGetter<double>, _1, boost::ref(_period))));
}

CSimpleManager::~CSimpleManager(void) {
}

bool CSimpleManager::IsDecisionEpoch(double time) const {
	if (time >= _nextDecisionTime) {
		return true;
	}
	return false;
}

void CSimpleManager::Decision(double time, vector<CLoadBase*> pLoads, vector<CBankBase*> pBanks, vector<CSourceBase*> pSources, vector<CConverterBase*> pConverters, vector<CCTI*> pCTIs) {
	_nextDecisionTime += _period;
}

void CSimpleManager::Reset() {
	_nextDecisionTime = 0;
}

double CSimpleManager::NextTimeStep(double time, int precision) const {
	if(_nextDecisionTime - time > 0) {
		return _nextDecisionTime - time;
	} else {
		return 0;
	}
}

void CSimpleManager::TimeElapse(double time, double timeElapsed) {
}

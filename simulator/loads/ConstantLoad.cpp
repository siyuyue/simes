// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "ConstantLoad.h"

CConstantLoad::CConstantLoad(void)
{
	_voltage = 1;
	_current = 1;

	// Add properties
	_AddProperty(new CProperty("voltage", "Load voltage.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_voltage)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_voltage), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("current", "Load current.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_current)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_current), (boost::lambda::_1 > 0))));
	_AddProperty(new CProperty("consumption", "Total energy drawn from the storage bank.",
		boost::bind(SimpleGetter<double>, _1, boost::ref(_consumption))));
}

CConstantLoad::~CConstantLoad()
{
}

void CConstantLoad::Reset()
{
	_consumption = 0;
}

double CConstantLoad::PortDefaultCurrent(double time) const
{
	return _current;
}

double CConstantLoad::PortVoltage(double time, double current) const
{
	return _voltage;
}

double CConstantLoad::MaxOutPortCurrent(double time) const
{
	return 0;
}

double CConstantLoad::NextTimeStep(double time, int precision) const
{
	return INF;
}

void CConstantLoad::TimeElapse(double time, double timeElapsed)
{
	_consumption += timeElapsed * _voltage * _portCurrent;
}

double CConstantLoad::GetConsumption() const
{
	return _consumption;
}

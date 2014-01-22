// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "banks/Grid.h"
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/lambda/lambda.hpp>

CGrid::CGrid(void) {
	_voltage = 110;
    _maxPortCurrent = 5;

    _AddProperty(new CProperty("voltage", "Terminal voltage.",
        boost::bind(CheckSetter<double>, _1, boost::ref(_voltage), (boost::lambda::_1 > 0)),
        boost::bind(SimpleGetter<double>, _1, boost::ref(_voltage))));
    _AddProperty(new CProperty("max_output_current", "Max output current.",
        boost::bind(CheckSetter<double>, _1, boost::ref(_maxPortCurrent), (boost::lambda::_1 > 0)),
        boost::bind(SimpleGetter<double>, _1, boost::ref(_maxPortCurrent))));
    _AddProperty(new CProperty("consumption", "Total energy drawn from the grid.",
        boost::bind(SimpleGetter<double>, _1, boost::ref(_consumption))));
}

CGrid::~CGrid(void) {
}

double CGrid::GetStateofCharge() const {
	return 1;
}

double CGrid::PortVoltage(double time, double current) const {
	return _voltage;
}

double CGrid::MaxOutPortCurrent(double time) const {
	return _maxPortCurrent;
}

void CGrid::Reset() {
    _consumption = 0;
}

double CGrid::NextTimeStep(double time, int precision) const {
	return INF;
}

void CGrid::TimeElapse(double time, double timeElapsed) {
    _consumption -= _portCurrent * _voltage * timeElapsed;
}
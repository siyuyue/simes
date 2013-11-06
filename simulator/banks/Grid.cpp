// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "Grid.h"

CGrid::CGrid(void)
{
	_voltage = 110;
}

CGrid::~CGrid(void)
{

}

double CGrid::GetStateofCharge() const
{
	return 1;
}

double CGrid::PortVoltage(double time, double current) const
{
	return _voltage;
}

double CGrid::MaxOutPortCurrent(double time) const
{
	return 5;
}

void CGrid::Reset()
{

}

double CGrid::NextTimeStep(double time, int precision) const
{
	return INF;
}

void CGrid::TimeElapse(double time, double timeElapsed)
{

}
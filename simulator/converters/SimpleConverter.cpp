// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "SimpleConverter.h"

CSimpleConverter::CSimpleConverter(void)
{
	_efficiency = 1;

	_AddProperty(new CProperty("efficiency", "Power conversion efficiency.",
		boost::bind(CheckSetter<double>, _1, boost::ref(_efficiency), (boost::lambda::_1 > 0)),
		boost::bind(SimpleGetter<double>, _1, boost::ref(_efficiency))));
}

CSimpleConverter::~CSimpleConverter()
{
}

double CSimpleConverter::FindInputCurrent(double time, bool isInputPortA, double inputVoltage, double outputVoltage, double outputCurrent) const
{
	double inputPower;
	double outputPower;
	
	outputPower = outputVoltage * outputCurrent;
	inputPower = outputPower / _efficiency;
	return inputPower / inputVoltage;
}

double CSimpleConverter::FindOutputCurrent(double time, bool isOutputPortA, double inputVoltage, double outputVoltage, double inputCurrent) const
{
	double inputPower;
	double outputPower;

	inputPower = inputCurrent * inputVoltage;
	outputPower = inputPower * _efficiency;
	return outputPower / outputVoltage;
}

void CSimpleConverter::Reset()
{
}

double CSimpleConverter::NextTimeStep(double time, int precision) const
{
	return INF;
}

void CSimpleConverter::TimeElapse(double time, double timeElapsed)
{
}

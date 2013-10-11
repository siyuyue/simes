#include "SimpleConverter.h"

CSimpleConverter::CSimpleConverter(void)
{
	_efficiency = 1;
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

bool CSimpleConverter::SetProperty(const string &name, const string &value)
{
	if( name == string("efficiency") )
	{
		_efficiency = FromString<double>(value);
		return true;
	}
	return false;
}

string CSimpleConverter::GetProperty(const string &name) const
{
	if( name == string("efficiency") )
	{
		return ToString<double>(_efficiency);
	}
    return CConverterBase::GetProperty(name);
}
// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "LUTConverter.h"
#include "Simulator.h"
#include "SimException.h"

CLUTConverter::CLUTConverter(void)
{
    _pLUT = NULL;
}

CLUTConverter::~CLUTConverter()
{
    delete _pLUT;
}

double CLUTConverter::FindOutputCurrent(double time, bool isOutputPortA, double inputVoltage, double outputVoltage, double inputCurrent) const
{
    if( _isInputCurrent )
    {
        return inputVoltage*inputCurrent*_pLUT->LookUp(inputVoltage, outputVoltage, inputCurrent )/outputVoltage;
    }
    else
    {
        // Binary search
        double xl = 0;
        double xr = 10000;
        double fl = FindInputCurrent(time, !isOutputPortA, inputVoltage, outputVoltage, xl) - inputCurrent;
        double fr = FindInputCurrent(time, !isOutputPortA, inputVoltage, outputVoltage, xr) - inputCurrent;
        if( (fl > 0 && fr > 0) || (fl < 0 && fr < 0))
        {
            throw CSimException(GetName().c_str(), "Binary Search Failed.");
        }
        while( xr - xl > EPS )
        {
            double xm = (xr + xl ) / 2;
            double fm = FindInputCurrent(time, !isOutputPortA, inputVoltage, outputVoltage, xm) - inputCurrent;
            if( (fl > 0 && fm > 0) || (fl < 0 && fm < 0) )
            {
                xl = xm;
                fl = fm;
            }
            else
            {
                xr = xm;
                fr = fm;
            }
        }
        return xl;
    }
}

double CLUTConverter::FindInputCurrent(double time, bool isInputPortA, double inputVoltage, double outputVoltage, double outputCurrent) const
{
    if( _isInputCurrent )
    {
        // Binary search
        double xl = 0;
        double xr = 10000;
        double fl = FindOutputCurrent(time, !isInputPortA, inputVoltage, outputVoltage, xl) - outputCurrent;
        double fr = FindOutputCurrent(time, !isInputPortA, inputVoltage, outputVoltage, xr) - outputCurrent;
        if( (fl > 0 && fr > 0) || (fl < 0 && fr < 0))
        {
            throw CSimException(GetName().c_str(),"Binary Search Failed.");
        }
        while( xr - xl > EPS )
        {
            double xm = (xr + xl ) / 2;
            double fm = FindOutputCurrent(time, !isInputPortA, inputVoltage, outputVoltage, xm) - outputCurrent;
            if( (fl > 0 && fm > 0) || (fl < 0 && fm < 0) )
            {
                xl = xm;
                fl = fm;
            }
            else
            {
                xr = xm;
                fr = fm;
            }
        }
        return xl;
    }
    else
    {
        return outputVoltage*outputCurrent/_pLUT->LookUp(inputVoltage, outputVoltage, outputCurrent )/inputVoltage;
    }
}

void CLUTConverter::Reset()
{

}

double CLUTConverter::NextTimeStep(double time, int precision) const
{
    return INF;
}

void CLUTConverter::TimeElapse(double time, double timeElapsed)
{
    ;
}

bool CLUTConverter::SetProperty(const string &name, const string &value)
{
    if( name == string("input_current_lut") )               // Input current is the input to the LUT table
	{
        _isInputCurrent = true;
	}
    else if( name == string("output_current_lut") )         // Output current is the input to the LUT table
	{
        _isInputCurrent = false;
    }
	else
	{
		return false;
	}
    ifstream inputFile(string(GetSimulator()->GetPathPrefix() + value).c_str());
    if(!inputFile)
    {
        return false;
    }
    _pLUT = new CLookUpTable(3, inputFile, false);
    return true;
}

void CLUTConverter::CheckIntegrity() const
{
    if( _pLUT == NULL )
    {
        throw CSimException(GetName().c_str(), "Converter lookup table is not set.");
    }
    return;
}
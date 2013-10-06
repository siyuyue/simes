// **********************************************
// Converter.cpp
// Implementation of all converter classes
// **********************************************
#include "Converter.h"
#include "Simulator.h"
#include "SimException.h"
#include <fstream>

// **********************************************
// CConverterBase
// **********************************************
CConverterBase::CConverterBase(void)
{
	_pPortA = NULL;
	_pPortB = NULL;
	_outputCurrent = 0;
	_inputCurrent = 0;
	_maxIter = 1000;
}

CConverterBase::~CConverterBase(void)
{
}

void CConverterBase::SetPortA(CPort *pPort)
{
	_pPortA = pPort;
	_pPortA->SetConverter(this, true);
}

void CConverterBase::SetPortB(CPort *pPort)
{
	_pPortB = pPort;
	_pPortB->SetConverter(this, false);
}

bool CConverterBase::CanPortBeOutput(bool isPortA) const
{
	if( isPortA )
	{
		return _pPortA->IsInPort() && _pPortB->IsOutPort();
	}
	else
	{
		return _pPortB->IsInPort() && _pPortA->IsOutPort();
	}
}

void CConverterBase::SetPortOutputCurrent(double time, bool isPortA, double current)
{
	if( isPortA )
	{
		if( !_pPortA->IsInPort() || !_pPortB->IsOutPort() )
		{
			// Error here, port A cannot be output.
            throw CSimException(GetName().c_str(),"Port A cannot be output!");
			return;
		}
		_isPortAOutput = true;
	}
	else
	{
		if( !_pPortB->IsInPort() || !_pPortA->IsOutPort() )
		{
			// Error here, port B cannot be output.
            throw CSimException(GetName().c_str(),"Port B cannot be output!");
			return;
		}
		_isPortAOutput = false;
	}
	_outputCurrent = current;
	_isOutputSet = true;
}

void CConverterBase::SetPortInputCurrent(double time, bool isPortA, double current)
{
	if( isPortA )
	{
		if( !_pPortB->IsInPort() || !_pPortA->IsOutPort() )
		{
			// Error here, port A cannot be input.
            throw CSimException(GetName().c_str(),"Port A cannot be input!");
			return;
		}
		_isPortAOutput = false;
	}
	else
	{
		if( !_pPortA->IsInPort() || !_pPortB->IsOutPort() )
		{
			// Error here, port B cannot be input.
            throw CSimException(GetName().c_str(),"Port B cannot be input!");
			return;
		}
		_isPortAOutput = true;
	}
	_inputCurrent = current;
	_isOutputSet = false;
}

void CConverterBase::Solve(double time)
{
	if( _isOutputSet )
	{
		double outputVoltage;
		CPort * pInputPort;
		CPort * pOutputPort;
		if(_isPortAOutput)
		{
			outputVoltage = _pPortA->PortVoltage(time, _outputCurrent);
			pOutputPort = _pPortA;
			pInputPort = _pPortB;
		}
		else
		{
			outputVoltage = _pPortB->PortVoltage(time, _outputCurrent);
			pOutputPort = _pPortB;
			pInputPort = _pPortA;
		}
		// Solve by iteration
		int count = 0;
        double inputCurrentPre = _inputCurrent + 100;
		while( count < _maxIter && abs(_inputCurrent - inputCurrentPre) > EPS )
		{
			inputCurrentPre = _inputCurrent;
			_inputCurrent = FindInputCurrent(time, !_isPortAOutput, pInputPort->PortVoltage(time, -_inputCurrent), outputVoltage, _outputCurrent);
			count ++;
		}
		if( count == _maxIter)
		{
            throw CSimException(GetName().c_str(),"Max number of iterations exceeded!");
		}
		pInputPort->SetPortCurrent(-_inputCurrent);
		pOutputPort->SetPortCurrent(_outputCurrent);
	}
	else
	{
		double inputVoltage;
		CPort * pInputPort;
		CPort * pOutputPort;
		if(_isPortAOutput)
		{
			inputVoltage = _pPortB->PortVoltage(time, _inputCurrent);
			pOutputPort = _pPortA;
			pInputPort = _pPortB;
		}
		else
		{
			inputVoltage = _pPortA->PortVoltage(time, _inputCurrent);
			pOutputPort = _pPortB;
			pInputPort = _pPortA;
		}
		// Solve by iteration
		int count = 0;
        double outputCurrentPre = _outputCurrent + 100;
		while( count < _maxIter && abs(_outputCurrent - outputCurrentPre) > EPS )
		{
			outputCurrentPre = _outputCurrent;
			_outputCurrent = FindOutputCurrent(time, _isPortAOutput, inputVoltage, pOutputPort->PortVoltage(time, _outputCurrent), _inputCurrent);
			count ++;
		}
		if( count == _maxIter)
		{
            throw CSimException(GetName().c_str(),"Max number of iterations exceeded!");
		}
		pInputPort->SetPortCurrent(-_inputCurrent);
		pOutputPort->SetPortCurrent(_outputCurrent);
	}
}

double CConverterBase::GetMaxInputPowerCurrent(double time, bool isPortA) const
{
	CPort * pOutPort;
	CPort * pInPort;
	if( isPortA )
	{
		pInPort = _pPortA;
		pOutPort = _pPortB;
	}
	else
	{
		pInPort = _pPortB;
		pOutPort = _pPortA;
	}
	if( !pOutPort->IsOutPort() )
	{
		return 0;
	}
	// Assume the output power of the port is unimodal, use tenary search to find the maximum
	double xl = 0;
	double xr = pOutPort->MaxOutPortCurrent(time);
	double xm1, xm2;
	double fl = 0;
	double fr = xr * pOutPort->PortVoltage(time, -xr);
	double fm1, fm2;
	while( xr - xl > EPS )
	{
		xm1 = (2*xl + xr ) / 3.;
		xm2 = (xl + 2*xr ) / 3.;
		fm1 = xm1 * pOutPort->PortVoltage(time, -xm1);
		fm2 = xm2 * pOutPort->PortVoltage(time, -xm2);
		if( fm1 > fm2 )
		{
			xr = xm2;
			fr = fm2;
		}
		else
		{
			xl = xm1;
			fl = fm1;
		}
	}
	return xl;
}

string CConverterBase::GetProperty(const string &name) const
{
    if( name == string("input_current") )
    {
        return ToString<double>(_inputCurrent);
    }
    else if( name == string("output_current") )
    {
        return ToString<double>(_outputCurrent);
    }
    return string();
}

bool CConverterBase::SetSensor(const string &name, CSensor &sensor)
{
    if( name == string("input_current") )
    {
        sensor.SetPointer(&_inputCurrent);
        return true;
    }
    else if( name == string("output_current") )
    {
        sensor.SetPointer(&_outputCurrent);
        return true;
    }
    return false;
}

// **********************************************
// CSimpleConverter
// **********************************************
CSimpleConverter::CSimpleConverter(void)
{
	_efficiency = 0.9;
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

// **********************************************
// CDCDCConverter Class
// **********************************************
CDCDCConverter::CDCDCConverter(void)
{
	_scale = 1;
	_fixed = 0.5;
	_vari_buck = 6;
	_vari_boost = 6;

	_L_f = 10e-6;
	double Rsw = 25e-3/2 * _scale;
	double Qsw = 60e-9/4 * _scale;
	_R_l = 39e-3/2 * _scale;
	_R_c = 300e-3/2 * _scale;
	_F_s = 500e3;
	_I_cont = 4e-3/4* _scale;

	_R_sw1 = Rsw;
	_R_sw2 = Rsw;
	_R_sw3 = Rsw;
	_R_sw4 = Rsw;
	_Q_sw1 = Qsw;
	_Q_sw2 = Qsw;
	_Q_sw3 = Qsw;
	_Q_sw4 = Qsw;
}

CDCDCConverter::~CDCDCConverter(void)
{
	;
}

double CDCDCConverter::FindOutputCurrent(double time, bool isOutputPortA, double inputVoltage, double outputVoltage, double inputCurrent) const
{
	// Binary search
	double xl = 0;
	double xr = 10000;
	double fl = FindInputCurrent(time, !isOutputPortA, inputVoltage, outputVoltage, xl) - inputCurrent;
	double fr = FindInputCurrent(time, !isOutputPortA, inputVoltage, outputVoltage, xr) - inputCurrent;
	if( (fl > 0 && fr > 0) || (fl < 0 && fr < 0))
	{
		// Err...
        throw CSimException(GetName().c_str(),"Binary Search Failed.");
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

double CDCDCConverter::FindInputCurrent(double time, bool isInputPortA, double inputVoltage, double outputVoltage, double outputCurrent) const
{
	if( outputCurrent < EPS )
	{
		return 0;
	}
	// Buck
	if( outputVoltage <= inputVoltage )
	{
		double k = outputVoltage / inputVoltage;
		double dI = outputVoltage * (inputVoltage - outputVoltage) / ( _F_s * _L_f * inputVoltage );
		double P_conduction_dc = outputCurrent * outputCurrent * (k*_R_sw1 + (1 - k)*_R_sw2 + _R_sw3 + _R_l);
		double P_conduction_ac = dI*dI/12 * (k*_R_sw1 + (1 - k)*_R_sw2 + _R_sw3 + _R_l + _R_c);
		double P_switching = inputVoltage*_F_s*( _Q_sw1 + _Q_sw2 );
		double P_conduction = P_conduction_dc + P_conduction_ac;
		double P_controller = inputVoltage*_I_cont;
		double P_tot_buck = _vari_buck * P_conduction + _fixed * (P_switching + P_controller);

		return (outputCurrent*outputVoltage + P_tot_buck) / inputVoltage;
	}
	else
	{
		// Boost
		double k = 1 - inputVoltage/outputVoltage;
		double dI = inputVoltage * (outputVoltage-inputVoltage) / (_F_s * _L_f * outputVoltage);
		double P_conduction_dc = (outputCurrent/(1-k)) * (outputCurrent/(1-k)) * ( k*_R_sw3 + (1 - k)*_R_sw4 + _R_sw1 + _R_l + k*(1 - k)*_R_c);
		double P_conduction_ac = dI*dI/12*(k*_R_sw3 + (1 - k)*_R_sw4 + _R_sw1 + _R_l + (1 - k)*_R_c);
		double P_switching = outputVoltage * _F_s * (_Q_sw3 + _Q_sw4);
		double P_conduction = 1*(P_conduction_dc + P_conduction_ac);
		double P_controller = inputVoltage * _I_cont;
		double P_tot_boost = _vari_boost*P_conduction + _fixed*(P_switching + P_controller);

		return (outputCurrent*outputVoltage + P_tot_boost) / inputVoltage;
	}
}

void CDCDCConverter::Reset()
{

}

double CDCDCConverter::NextTimeStep(double time, int precision) const
{
	return INF;
}

void CDCDCConverter::TimeElapse(double time, double timeElapsed)
{
	;
}

bool CDCDCConverter::SetProperty(const string &name, const string &value)
{
	if( name == string("scale") )
	{
		_scale = FromString<double>(value);
	}
	else if( name == string("fixed") )
	{
		_fixed = FromString<double>(value);
		return true;
	}
	else
	{
		return false;
	}
	// Update these parameters
	_L_f = 10e-6;
	double Rsw = 25e-3/2 * _scale;
	double Qsw = 60e-9/4 * _scale;
	_R_l = 39e-3/2 * _scale;
	_R_c = 300e-3/2 * _scale;
	_F_s = 500e3;
	_I_cont = 4e-3/4* _scale;

	_R_sw1 = Rsw;
	_R_sw2 = Rsw;
	_R_sw3 = Rsw;
	_R_sw4 = Rsw;
	_Q_sw1 = Qsw;
	_Q_sw2 = Qsw;
	_Q_sw3 = Qsw;
	_Q_sw4 = Qsw;

	return true;
}

// **********************************************
// CLUTConverter Class
// **********************************************
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
            // Err...
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
            // Err...
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
        throw CSimException(GetName().c_str(), "Converter lookup table is not set.");
    return;
}

// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "converters/DCDCConverter.h"
#include "core/SimException.h"

CDCDCConverter::CDCDCConverter(void) {
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

	// Add properties
	_AddProperty(new CProperty("scale", "Some parameter.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_scale)),
		boost::bind(&CDCDCConverter::SetScaleParameter, this, _1)));
	_AddProperty(new CProperty("fixed", "Some parameter.",
		InvalidSetter,
		boost::bind(SimpleGetter<double>, _1, boost::ref(_fixed)),
		boost::bind(CheckSetter<double>, _1, boost::ref(_fixed), (boost::lambda::_1 > 0))));
}

CDCDCConverter::~CDCDCConverter(void) {
}

double CDCDCConverter::FindOutputCurrent(double time, bool isOutputPortA, double inputVoltage, double outputVoltage, double inputCurrent) const {
	// Binary search
	double xl = 0;
	double xr = 10000;
	double fl = FindInputCurrent(time, !isOutputPortA, inputVoltage, outputVoltage, xl) - inputCurrent;
	double fr = FindInputCurrent(time, !isOutputPortA, inputVoltage, outputVoltage, xr) - inputCurrent;
	if ((fl > 0 && fr > 0) || (fl < 0 && fr < 0)) {
        throw CSimException(GetName().c_str(),"Binary Search Failed.");
	}
	while (xr - xl > EPS) {
		double xm = (xr + xl ) / 2;
		double fm = FindInputCurrent(time, !isOutputPortA, inputVoltage, outputVoltage, xm) - inputCurrent;
		if( (fl > 0 && fm > 0) || (fl < 0 && fm < 0) ) {
			xl = xm;
			fl = fm;
		} else {
			xr = xm;
			fr = fm;
		}
	}
	return xl;
}

double CDCDCConverter::FindInputCurrent(double time, bool isInputPortA, double inputVoltage, double outputVoltage, double outputCurrent) const {
	if (outputCurrent < EPS) {
		return 0;
	}
	if (outputVoltage <= inputVoltage) {
		// Buck
		double k = outputVoltage / inputVoltage;
		double dI = outputVoltage * (inputVoltage - outputVoltage) / ( _F_s * _L_f * inputVoltage );
		double P_conduction_dc = outputCurrent * outputCurrent * (k*_R_sw1 + (1 - k)*_R_sw2 + _R_sw3 + _R_l);
		double P_conduction_ac = dI*dI/12 * (k*_R_sw1 + (1 - k)*_R_sw2 + _R_sw3 + _R_l + _R_c);
		double P_switching = inputVoltage*_F_s*( _Q_sw1 + _Q_sw2 );
		double P_conduction = P_conduction_dc + P_conduction_ac;
		double P_controller = inputVoltage*_I_cont;
		double P_tot_buck = _vari_buck * P_conduction + _fixed * (P_switching + P_controller);

		return (outputCurrent*outputVoltage + P_tot_buck) / inputVoltage;
	} else {
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

void CDCDCConverter::Reset() {

}

double CDCDCConverter::NextTimeStep(double time, int precision) const {
	return INF;
}

void CDCDCConverter::TimeElapse(double time, double timeElapsed) {
}

bool CDCDCConverter::SetScaleParameter(const string& s) {
	double scaleTemp = FromString<double>(s);
	if (scaleTemp <= 0) {
		return false;
	}
	_scale = scaleTemp;
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

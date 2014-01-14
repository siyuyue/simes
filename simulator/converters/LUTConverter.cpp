// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "converters/LUTConverter.h"
#include "core/Simulator.h"
#include "core/SimException.h"

CLUTConverter::CLUTConverter(void) {
    _pLUT = NULL;

    _AddProperty(new CProperty("input_current_lut", "Set LUT with input current as the input to the LUT table.",
        InvalidSetter,
        InvalidGetter,
        boost::bind(&CLUTConverter::SetLUT, this, _1, true)));
    _AddProperty(new CProperty("output_current_lut", "Set LUT with output current as the input to the LUT table.",
        InvalidSetter,
        InvalidGetter,
        boost::bind(&CLUTConverter::SetLUT, this, _1, false)));
}

CLUTConverter::~CLUTConverter() {
    delete _pLUT;
}

double CLUTConverter::FindOutputCurrent(double time, bool isOutputPortA, double inputVoltage, double outputVoltage, double inputCurrent) const {
    if (_isInputCurrent) {
        return inputVoltage*inputCurrent*_pLUT->LookUp(inputVoltage, outputVoltage, inputCurrent )/outputVoltage;
    } else {
        // Binary search
        double xl = 0;
        double xr = 10000;
        double fl = FindInputCurrent(time, !isOutputPortA, inputVoltage, outputVoltage, xl) - inputCurrent;
        double fr = FindInputCurrent(time, !isOutputPortA, inputVoltage, outputVoltage, xr) - inputCurrent;
        if ((fl > 0 && fr > 0) || (fl < 0 && fr < 0)) {
            throw CSimException(GetName().c_str(), "Binary Search Failed.");
        }
        while (xr - xl > EPS) {
            double xm = (xr + xl ) / 2;
            double fm = FindInputCurrent(time, !isOutputPortA, inputVoltage, outputVoltage, xm) - inputCurrent;
            if ((fl > 0 && fm > 0) || (fl < 0 && fm < 0)) {
                xl = xm;
                fl = fm;
            } else {
                xr = xm;
                fr = fm;
            }
        }
        return xl;
    }
}

double CLUTConverter::FindInputCurrent(double time, bool isInputPortA, double inputVoltage, double outputVoltage, double outputCurrent) const {
    if (_isInputCurrent) {
        // Binary search
        double xl = 0;
        double xr = 10000;
        double fl = FindOutputCurrent(time, !isInputPortA, inputVoltage, outputVoltage, xl) - outputCurrent;
        double fr = FindOutputCurrent(time, !isInputPortA, inputVoltage, outputVoltage, xr) - outputCurrent;
        if ((fl > 0 && fr > 0) || (fl < 0 && fr < 0)) {
            throw CSimException(GetName().c_str(),"Binary Search Failed.");
        }
        while (xr - xl > EPS) {
            double xm = (xr + xl ) / 2;
            double fm = FindOutputCurrent(time, !isInputPortA, inputVoltage, outputVoltage, xm) - outputCurrent;
            if ((fl > 0 && fm > 0) || (fl < 0 && fm < 0)) {
                xl = xm;
                fl = fm;
            } else {
                xr = xm;
                fr = fm;
            }
        }
        return xl;
    } else {
        return outputVoltage*outputCurrent/_pLUT->LookUp(inputVoltage, outputVoltage, outputCurrent )/inputVoltage;
    }
}

void CLUTConverter::Reset() {
}

double CLUTConverter::NextTimeStep(double time, int precision) const {
    return INF;
}

void CLUTConverter::TimeElapse(double time, double timeElapsed) {
}

bool CLUTConverter::CheckIntegrity() const {
    if (_pLUT == NULL) {
        throw CSimException(GetName().c_str(), "Converter lookup table is not set.");
    }
    return true;
}

bool CLUTConverter::SetLUT(const string &s, bool isInputCurrent) {
    _isInputCurrent = isInputCurrent;
    ifstream inputFile(string(GetSimulator()->GetPathPrefix() + s).c_str());
    if (!inputFile) {
        return false;
    }
    _pLUT = new CLookUpTable(3, inputFile, false);
    return true;
}
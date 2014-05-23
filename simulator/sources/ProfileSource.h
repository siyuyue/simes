// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#pragma once

#include <vector>
#include "sources/Source.h"

class CProfileSource:public CSourceBase {
private:
    double _voltage;                                                // Load voltage
    vector<double> _time;                                           // Time
    vector<double> _current;                                        // Load Current
    int _idx;                                                       // idx of current <time, current> pair
    double _generation;                                             // Record the overall generation
    double _powerNow;                                               // Current power
public:
    CProfileSource(void);
    virtual ~CProfileSource();
    double GetEnergyGeneration();

    virtual double PortVoltage(double time, double current) const;  // Inherited from CPort
    virtual double MaxOutPortCurrent(double time) const;
    virtual void Reset();                                           // Inherited from CComponent
    virtual double NextTimeStep(double time, int precision) const;
    virtual void TimeElapse(double time, double timeElapsed);
    virtual bool CheckIntegrity() const;
    bool SetCurrentProfile(const string &s);
};
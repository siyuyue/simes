// **********************************************
// CLookUpTable class
// A general n-dimensional lookup table, use linear interpolation
// **********************************************
#pragma once

#include "config.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class CLookUpTable
{
private:
	int _dimension;
	vector<int> _indexCount;
    vector< vector<double> > _indices;
	vector<double> _values;

	bool _usingExtrapolate;
    int *_n;
    double *_coeffLeft;
    double *_coeffRight;
public:
    CLookUpTable(int dimension, ifstream &inputFile, bool extrapolate);
	~CLookUpTable(void);
	double LookUp(double idx);
	double LookUp(double idx, double idy);
	double LookUp(double idx, double idy, double idz);
	double LookUp(vector<double> ids);
};


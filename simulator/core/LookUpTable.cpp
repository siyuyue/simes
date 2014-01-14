// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "core/LookUpTable.h"
#include "core/SimException.h"

CLookUpTable::CLookUpTable(int dimension, ifstream &inputFile, bool extrapolate) {
	_dimension = dimension;
	_indexCount.resize(_dimension);
	_indices.resize(_dimension);
	int total = 1;
	for (int i=0; i<_dimension; i++) {
		inputFile >> _indexCount[i];
		total *= _indexCount[i];
	}
	_values.resize(total);
	for (int i=0; i<_dimension; i++) {
		_indices[i].resize(_indexCount[i]);
		for (int j=0; j<_indexCount[i]; j++) {
			inputFile >> _indices[i][j];
		}
	}

	for(int i=0; i<total; i++) {
		inputFile >> _values[i];
	}

	_usingExtrapolate = extrapolate;

    _n = new int[_dimension];
    _coeffLeft = new double[_dimension];
    _coeffRight = new double[_dimension];
}

CLookUpTable::~CLookUpTable(void) {
    delete[] _n;
    delete[] _coeffLeft;
    delete[] _coeffRight;
}

double CLookUpTable::LookUp(double idx) {
	vector<double> ids;
	ids.push_back(idx);
	return LookUp(ids);
}

double CLookUpTable::LookUp(double idx, double idy) {
	vector<double> ids;
	ids.push_back(idx);
	ids.push_back(idy);
	return LookUp(ids);
}

double CLookUpTable::LookUp(double idx, double idy, double idz) {
	vector<double> ids;
	ids.push_back(idx);
	ids.push_back(idy);
	ids.push_back(idz);
	return LookUp(ids);
}

double CLookUpTable::LookUp(vector<double> ids) {
    if (ids.size() != _dimension) {
       throw CSimException("LookupTable","Dimension does not match.");
    }
	int left, right, mid;

	for (int i = 0; i < _dimension; i ++) {
		left = 0, right = _indexCount[i] - 1;

		while (right - left > 1) {
			mid = (left + right) / 2;
			if (_indices[i][mid] > ids[i]) {
				right = mid;
			} else {
				left = mid;
			}
		}
        _n[i] = left;
        _coeffLeft[i] = (_indices[i][right] - ids[i]) / (_indices[i][right] - _indices[i][left]);
        _coeffRight[i] = (ids[i] - _indices[i][left]) / (_indices[i][right] - _indices[i][left]);

		if (!_usingExtrapolate) {
            if (_coeffLeft[i] < 0) {
                _coeffLeft[i] = 0;
                _coeffRight[i] = 1;
			} else if(_coeffRight[i] < 0)	{
                _coeffLeft[i] = 1;
                _coeffRight[i] = 0;
			}
		}
	}

	int number = 1 << _dimension;
	double returnValue = 0;
	for (int i = 0; i < number; i ++) {
		int oneDimensionIndex = 0;
		int count = 1;
		double coeff = 1;
		for(int j = 0; j < _dimension; j ++) {
			if (((i >> j) & 1) == 0) {
                oneDimensionIndex += _n[_dimension - 1 - j] * count;
                coeff *= _coeffLeft[_dimension - 1 - j];
			} else {
                oneDimensionIndex += (_n[_dimension - 1 - j] + 1) * count;
                coeff *= _coeffRight[_dimension - 1 - j];
			}
			count *= _indexCount[_dimension - 1 - j];
		}
		returnValue += coeff * _values[oneDimensionIndex];
	}
	return returnValue;
}

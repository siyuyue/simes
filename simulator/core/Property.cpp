// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "core/Property.h"

CProperty::CProperty(string key, string description, GetFunction getter)
    : _key(key), _description(description), _setter(InvalidSetter), _getter(getter), _initialSetter(InvalidSetter) {
}

CProperty::CProperty(string key, string description, SetFunction setter, GetFunction getter)
    : _key(key), _description(description), _setter(setter), _getter(getter), _initialSetter(setter) {
}

CProperty::CProperty(string key, string description, SetFunction setter, GetFunction getter, SetFunction initialSetter)
    : _key(key), _description(description), _setter(setter), _getter(getter), _initialSetter(initialSetter) {
}

CProperty::~CProperty() {
}

bool CProperty::Set(const string &value) const {
    return _setter(value);
}

bool CProperty::SetInitial(const string &value) const {
    return _initialSetter(value);
}

bool CProperty::Get(string &value) const {
    return _getter(value);
}

string CProperty::GetKey() const {
    return _key;
}

string CProperty::GetDescription() const {
    return _description;
}

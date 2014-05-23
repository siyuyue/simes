// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
// CParser class:
// - Reads in user written HEES System Simulation configuration file in XML format.
// - Generate a class representation of the HEES system and return a CSimulator class.
// TinyXML is used as the internal XML parser: http://www.grinninglizard.com/tinyxml/
// **********************************************
#pragma once

#include <queue>
#include "tinyxml.h"
#include "core/config.h"
#include "core/Simulator.h"

using namespace std;

class CParser {
private:
	TiXmlDocument _doc;						// TinyXML Parser
	CSimulator *_pSimulator;				// Allocated in Parse() but will not be deallocated in ~CParser()
	vector<CCommand> *_pCommandList;		// All commands sorted by time
public:
	CParser(void);
	~CParser(void);
    void Parse(const string &filename, bool parseCommands = true);			// Parse a input .xml file
	CSimulator * GetSimulator();			// Get simulator pointer
	vector<CCommand> * GetCommandList();	// Get commands list
};


// **********************************************
// Parser.h
// Definition of CParser class
// Input configuration is written in XML format
// TinyXML is used as XML parser: http://www.grinninglizard.com/tinyxml/
// **********************************************
#pragma once

#include "config.h"
#include "tinyxml.h"
#include "Simulator.h"
#include <queue>

using namespace std;

// **********************************************
// CParser class:
// - Reads in user written HEES System Simulation configuration file in XML format.
// - Generate a class representation of the HEES system and return a CSimulator class.
// **********************************************
class CParser
{
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


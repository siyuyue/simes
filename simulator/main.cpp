// **********************************************
// main.cpp
// Entrance of the program
// **********************************************
#include <iostream>
#include <vector>
#include <string>
#include "Converter.h"
#include "CTI.h"
#include "Bank.h"
#include "Load.h"
#include "Simulator.h"
#include "ChargeManager.h"
#include "Source.h"
#include "Parser.h"
#include "config.h"
#include "SimException.h"

using namespace std;

int main(int argc, char* argv[])
{
    if( argc <= 1 )
    {
        cout << "Usage : " << argv[0] << " inputfilename [options]." << endl;
        return -1;
    }
    bool interactiveMode = false;
    // Parse options
    for(int i=2; i<argc; i++)
    {
        if( argv[i][0] == '-' )
        {
            if( argv[i][1] == 'i' )
            {
                interactiveMode = true;
            }
        }
    }
    CParser parser;
    CSimulator *pSimulator = NULL;
    vector<CCommand> * pCommandList = NULL;
    try
    {
        parser.Parse(argv[1]);
        pSimulator = parser.GetSimulator();
        if( pSimulator == NULL )
        {
            throw CSimException("Main","Parsing failed!");
        }
        pSimulator->CheckIntegrity();

        pSimulator->Reset();
        if( interactiveMode )
        {
            string line;
            CCommand command;
            while( command.type != CCommand::FINISH )
            {
                getline(cin, line);
                command = CCommand::FromString(line);
                if( command.type != CCommand::SIMULATE )
                {
                    command.time = pSimulator->GetTime();
                }
                if( command.type == CCommand::INVALID )
                {
                    cout << "Cannot recognize command." << endl;
                }
                else
                {
                    pSimulator->IssueCommand(command);
                }
            }
        }
        else
        {
            pCommandList = parser.GetCommandList();
            for(vector<CCommand>::iterator it = pCommandList->begin(); it != pCommandList->end(); it++)
            {
                pSimulator->IssueCommand(*it);
            }
        }
    }
    catch(CSimException &e)
    {
        cout << "[Error!]" << e.componentName << ": " << e.exceptionMessage << endl;
    }

	delete pSimulator;
	delete pCommandList;

	return 0;
}
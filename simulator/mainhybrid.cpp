// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
//
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
    for(int i=1; i<argc; i++)
    {
        if( argv[i][0] == '-' )
        {
            if( argv[i][1] == 'i' )
            {
                interactiveMode = true;
            }
            if( argv[i][1] == 'h' )
            {
                cout << "HEES System Simulator." << endl;
                cout << "Usage : " << argv[0] << " inputfilename [options]." << endl;
                cout << "Options:" << endl;
                cout << "    -h: Help." << endl;
                cout << "    -i: Interactive mode." << endl;
                return 0;
            }
        }
    }
    CParser parser;
    CSimulator *pSimulator = NULL;
    vector<CCommand> *pCommandList = NULL;
    double finalSCVoltage, iniSCVoltage;
    try
    {
        parser.Parse(argv[1]);
        pSimulator = parser.GetSimulator();
        if( pSimulator == NULL )
        {
            throw CSimException("Main", "Parsing failed!");
        }
        pSimulator->CheckIntegrity();

        pSimulator->Reset();

        iniSCVoltage = FromString<double>(pSimulator->GetComponentProperty("SC", "voltage"));
        if( interactiveMode )
        {
            string line;
            CCommand command;
            while( command.type != CCommand::FINISH )
            {
                cout << "[Simes](Time = " << pSimulator->GetTime() << "s):";
                getline(cin, line);
                command = CCommand::FromString(line);
                command.time += pSimulator->GetTime();
                if( command.type == CCommand::INVALID )
                {
                    cout << "Cannot recognize command." << endl;
                }
                else
                {
                    if( !pSimulator->IssueCommand(command) )
                    {
                        cout << "Command failed to issue." << endl;
                    }
                    if( command.type == CCommand::GET )
                    {
                        cout << command.targetName << "." << command.propertyName << ":" << command.propertyValue << endl;
                    }
                }
            }
        }
        else
        {
            pCommandList = parser.GetCommandList();
            for(vector<CCommand>::iterator it = pCommandList->begin(); it != pCommandList->end(); it++)
            {
                if( !pSimulator->IssueCommand(*it) )
                {
                    cout << "Command failed to issue." << endl;
                }
                if( it->type == CCommand::GET )
                {
                    cout << it->targetName << "." << it->propertyName << ":" << it->propertyValue << endl;
                }
            }
        }
    }
    catch(CSimException &e)
    {
        cout << "[Error!]" << e.componentName << ": " << e.exceptionMessage << endl;
    }
    finalSCVoltage = FromString<double>(pSimulator->GetComponentProperty("SC", "voltage"));
    double totalPowerDrawn = FromString<double>(pSimulator->GetComponentProperty("Liion", "consumption"))
        + 0.5*FromString<double>(pSimulator->GetComponentProperty("SC", "capacitance"))*(iniSCVoltage * iniSCVoltage - finalSCVoltage * finalSCVoltage);
    double totalPowerConsumption = FromString<double>(pSimulator->GetComponentProperty("motor", "consumption"))
        - FromString<double>(pSimulator->GetComponentProperty("regen", "generation"));
    cout << "Simulation complete." << endl;
    cout << "Total consumption = " << totalPowerConsumption << endl;
    cout << "Total power drawn = " << totalPowerDrawn << endl;
    cout << "Efficiency = " << totalPowerConsumption / totalPowerDrawn << endl;

	delete pSimulator;
	delete pCommandList;

	return 0;
}

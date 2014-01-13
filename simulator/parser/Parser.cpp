// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include <string>
#include <sstream>
#include <algorithm>
#include "Parser.h"
#include "SimException.h"

using namespace std;

CParser::CParser(void)
{
	_pSimulator = NULL;
	_pCommandList = NULL;
}

CParser::~CParser(void)
{
}

void CParser::Parse(const string &filename, bool parseCommands)
{
    if(!_doc.LoadFile(filename))
	{
        throw CSimException("Parser","Unable to load file.");
	}
	TiXmlElement * pProjectElement = _doc.FirstChildElement("project");
	TiXmlElement * pNodeElement;
	if( pProjectElement == NULL )
	{
        throw CSimException("Parser","Unable to find <project> element.");
	}
	delete _pSimulator;
	_pSimulator = new CSimulator();
	pNodeElement = pProjectElement->FirstChildElement("name");
	if( pNodeElement != NULL )
	{
		_pSimulator->SetProjectName(pNodeElement->GetText());
	}
    size_t prefixPos = filename.find_last_of("/\\");
    string prefix;
    if(prefixPos != string::npos)
    {
        prefix = filename.substr(0, prefixPos + 1);
    }
    _pSimulator->SetPathPrefix(prefix);
	// First pass: add components
	for( pNodeElement = pProjectElement->FirstChildElement("comp"); pNodeElement != NULL; pNodeElement = pNodeElement->NextSiblingElement("comp"))
	{
		string type(pNodeElement->Attribute("type"));
		TiXmlElement * pNameElement = pNodeElement->FirstChildElement("name");
		if( pNameElement == NULL )
		{
			delete _pSimulator;
			_pSimulator = NULL;
            throw CSimException("Parser", "One of the components does not contain <name> element.");
		}

		TiXmlElement * pDerivedElement = pNodeElement->FirstChildElement("derived");
		if( type != string("cti") )
		{
			if( pDerivedElement == NULL )
			{
				delete _pSimulator;
				_pSimulator = NULL;
                throw CSimException("Parser", "One of the components other than cti does not contain <derived> element.");
			}
		}

		if( type == string("cti") )
		{
			_pSimulator->AddCTI(pNameElement->GetText(), CCTI::Create());
		}
		else if( type == string("bank"))
		{
			_pSimulator->AddBank(pNameElement->GetText(), CBankBase::Create(pDerivedElement->Attribute("type")));
		}
		else if( type == string("load") )
		{
			_pSimulator->AddLoad(pNameElement->GetText(), CLoadBase::Create(pDerivedElement->Attribute("type")));
		}
		else if( type == string("source") )
		{
			_pSimulator->AddSource(pNameElement->GetText(), CSourceBase::Create(pDerivedElement->Attribute("type")));
		}
		else if( type == string("converter") )
		{
			_pSimulator->AddConverter(pNameElement->GetText(), CConverterBase::Create(pDerivedElement->Attribute("type")));
		}
		else if( type == string("manager") )
		{
			_pSimulator->SetManager(pNameElement->GetText(), CChargeManagerBase::Create(pDerivedElement->Attribute("type")));
		}
	}
    // Second pass: set converter connections
	for( pNodeElement = pProjectElement->FirstChildElement("comp"); pNodeElement != NULL; pNodeElement = pNodeElement->NextSiblingElement("comp"))
	{
		string type(pNodeElement->Attribute("type"));
		TiXmlElement * pNameElement = pNodeElement->FirstChildElement("name");

		if( type == string("cti") )
		{
			;
		}
		else if( type == string("bank"))
		{
			;
		}
		else if( type == string("load") )
		{
			;
		}
		else if( type == string("source") )
		{
			;
		}
		else if( type == string("converter") )
		{
			TiXmlElement * pElementA = pNodeElement->FirstChildElement("port_a");
			TiXmlElement * pElementB = pNodeElement->FirstChildElement("port_b");
			if( pElementA == NULL || pElementB == NULL )
			{
				delete _pSimulator;
				_pSimulator = NULL;
                throw CSimException("Parser","One of the converters does not contain <port_a> or <port_b> element.");
			}
			_pSimulator->SetConverterPorts( pNameElement->GetText(), pElementA->GetText(), pElementB->GetText() );
		}
		else if( type == string("manager") )
		{
			;
		}
	}
    // Third pass: set derived type properties
	for( pNodeElement = pProjectElement->FirstChildElement("comp"); pNodeElement != NULL; pNodeElement = pNodeElement->NextSiblingElement("comp"))
	{
        TiXmlElement * pNameElement = pNodeElement->FirstChildElement("name");
        TiXmlElement * pDerivedElement = pNodeElement->FirstChildElement("derived");
        if( pDerivedElement != NULL )
        {
            TiXmlElement * pElement;
            for( pElement = pDerivedElement->FirstChildElement(); pElement != NULL; pElement = pElement->NextSiblingElement() )
            {
                _pSimulator->SetComponentPropertyInitial( pNameElement->GetText(), pElement->Value(), pElement->GetText());
            }
        }
	}
	// Fourth pass: add sensors
	for( pNodeElement = pProjectElement->FirstChildElement("sensor"); pNodeElement != NULL; pNodeElement = pNodeElement->NextSiblingElement("sensor") )
	{
		string target(pNodeElement->Attribute("target"));
		string prop(pNodeElement->Attribute("property"));
		_pSimulator->AddSensor(target, prop);
	}
	// Fifth pass: add all commands
	if( parseCommands )
	{
		delete _pCommandList;
		_pCommandList = new vector<CCommand>();
		for( pNodeElement = pProjectElement->FirstChildElement("cmd"); pNodeElement != NULL; pNodeElement = pNodeElement->NextSiblingElement("cmd") )
		{
			string type(pNodeElement->Attribute("type"));
			double time;
			pNodeElement->Attribute("time", &time);
			if( type == string("set") )
			{
				string targetName(pNodeElement->Attribute("target"));
				for(TiXmlElement * pElement = pNodeElement->FirstChildElement(); pElement != NULL; pElement = pElement->NextSiblingElement() )
				{
					CCommand command;
					command.type = CCommand::SET;
					command.time = time;
					command.targetName = targetName;
					command.propertyName = pElement->Value();
					command.propertyValue = pElement->GetText();
					_pCommandList->push_back(command);
				}
			}
			else if( type == string("get") )
			{
				string targetName(pNodeElement->Attribute("target"));
				for(TiXmlElement * pElement = pNodeElement->FirstChildElement(); pElement != NULL; pElement = pElement->NextSiblingElement() )
				{
					CCommand command;
					command.type = CCommand::GET;
					command.time = time;
					command.targetName = targetName;
					command.propertyName = pElement->Value();
					_pCommandList->push_back(command);
				}
			}
			else if( type == string("finish") )
			{
				CCommand command;
				command.type = CCommand::FINISH;
				command.time = time;
				_pCommandList->push_back(command);
			}
			else if( type == string("sim") )
			{
				CCommand command;
				command.type = CCommand::SIMULATE;
				command.time = time;
				_pCommandList->push_back(command);
			}
		}
		stable_sort(_pCommandList->begin(), _pCommandList->end());
	}

    // Misc
    pNodeElement = pProjectElement->FirstChildElement("sensor_interval");
    if( pNodeElement != NULL )
    {
        string text(pNodeElement->GetText());
        _pSimulator->SetSensorInterval(FromString<double>(text));
    }
    pNodeElement = pProjectElement->FirstChildElement("precision_level");
    if( pNodeElement != NULL )
    {
        string text(pNodeElement->GetText());
        _pSimulator->SetPrecisionLevel(FromString<int>(text));
    }

    return;
}

CSimulator * CParser::GetSimulator()
{
	return _pSimulator;
}

vector<CCommand> * CParser::GetCommandList()
{
	return _pCommandList;
}

/*
		CTU - Concrete Traces for Uppaal.
    Copyright (C) 2010  CTU team

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef _XMLLOADER__
#define _XMLLOADER__


#include <vector>
#include <string>
#include <map>
#include <list>



#include "symbTrace/Location.hpp"
#include "symbTrace/Edge.hpp"
#include "symbTrace/Node.hpp"
#include "symbTrace/lta.hpp"
#include "rationalDBM/dbm.hpp"
#include "rationalDBM/bound.hpp"
#include "main/Options.hpp"

#include <libxml/parser.h>

using namespace newdbm;
using std::map;
using std::string;
using std::vector;


using LTA::Lta;

typedef LTA::Edge Transition;//Rename to avoid confusion
typedef LTA::Location Node; //Rename to avoid confusion

typedef map<string,int> VarState;  // From Id to value

struct ProcessInfoKeeper
{
    map<string,string> localClocksNameToId;
    map<string,string> localVariablesNameToId;
};
struct EdgeInfoKeeper
{
    ProcessInfoKeeper* parentProcess;
    string guard;
    string sync;
    string update;
    string process;
};

struct LocationInfoKeeper
{
    ProcessInfoKeeper* parentProcess;
    string invariant;
    string name;
};


struct TransitionKeeper
{
    string from;
    string to;
    string edges;
};

struct NodeKeeper
{
    string dbm;
    string locationVector;
    string variableVector;
};

struct BoundKeeper {
  string first;
  string second;
  Bound bound;
};

struct DBMKeeper {
  string id;
  vector<BoundKeeper> bounds;
};

class XMLLoader
{
private:
    // All clock (global + local)
    map<string,string> systemVariablesNameToId;
    map<string,string> systemClocksNameToId;
    vector<ProcessInfoKeeper*> procInfoKeepers;
    
    vector<string> clocks;

    map<string,EdgeInfoKeeper> edges;
    map<string,LocationInfoKeeper> locations;

    map<string,VarState> variableStates;
    map<string,TransitionKeeper*> transitions;
    map<string,NodeKeeper> nodes;
  map<string,Node*> loopDetector;
  list<DBMKeeper> dbmKeeps;
  map<string,DBM> dbms;
    map<string,string> locationVectors;  // Giv et id for vectoren, og få vectorens indhold tilbage som string
    string initNode;
    xmlDocPtr doc;
  string zero;
    
  
  int nextClockId;
  map<string,int> clocksIndex;
  bool parseClock (xmlNodePtr );
  list<dc_t*>* CreateConstraints (string&, ProcessInfoKeeper*, VarState);
  bool parseDBM (xmlNodePtr);
    bool parseDocument (string&,xmlDocPtr*);
    bool parseEdge (xmlNodePtr, ProcessInfoKeeper*);
    bool parseGuard (xmlNodePtr,EdgeInfoKeeper&);
    bool parseLocation (xmlNodePtr,ProcessInfoKeeper* );
    bool parseLocationVector (xmlNodePtr);
    bool parseNode (xmlNodePtr);
    bool parseProcess (xmlNodePtr);
    bool parseProcessClock (xmlNodePtr, ProcessInfoKeeper*);
    bool parseProcessVariable (xmlNodePtr, ProcessInfoKeeper*);
    bool parseSync (xmlNodePtr, EdgeInfoKeeper&);
    bool parseSystem (xmlNodePtr);
    bool parseTrace (xmlNodePtr );
    bool parseTransition (xmlNodePtr);
    bool parseUpdate (xmlNodePtr, EdgeInfoKeeper&);
    bool parseVariable (xmlNodePtr);
    bool parseVariableState (xmlNodePtr,VarState&);
    bool parseVariableVector (xmlNodePtr);
  void createDBMS ();
  string getResets (VarState& var,map<string,string>&, map<string,string>&,  string&);
    Node* createNode (NodeKeeper &);
    Transition* createTransition (TransitionKeeper&);
    LTA::Lta* createLTA ();

    void freeMemoryIfPossible(xmlChar*);

public:
  LTA::Lta* generateLTA (string&,xmlDocPtr*,map<string,int>&);
    XMLLoader (string z) : zero(z) {
      nextClockId = 1;
    }
  
};




#endif

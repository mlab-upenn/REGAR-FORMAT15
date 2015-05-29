
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
#include <libxml/parser.h>
#include "xmlloader/XMLLoader.hpp"
#include <iostream>
#include <sstream>
#include <boost/regex.hpp>
#include <gmpxx.h>


#include "symbTrace/Location.hpp"
#include "symbTrace/Edge.hpp"
#include "symbTrace/Node.hpp"
#include "symbTrace/lta.hpp"

using namespace newdbm;

using std::stringstream;
using std::istringstream;
using std::ostringstream;
using std::stringstream;
using std::cout;
using std::cerr;
using std::endl;

int evaluate (map<string, string>& localVariables, map<string, string>& systemVariables, map<string,int>& varState, string &s);

//Function used to modify constraints expressed as arithmetic expressions to
//x-y [<|<=] n
dc_t* modifyConstraint (map<string, string>& localClocks, map<string, string>& systemClocks, map<string, string>& localVariables, map<string, string>& systemVariables, map<string,int>& varState, string &s,string& zero);

string removeInnerArith (string& s, map<string,string> &localVarsNameToId, map<string,string> &globalVarsNameToId,VarState& state) {
  size_t firstPos = s.find_first_of ('[');
  size_t lastPos = s.find_last_of (']');
  int value;
  
  if (firstPos!=string::npos && lastPos!=string::npos) { 
    string sh = s.substr (firstPos+1,s.size ()-(firstPos)-2);
    value = evaluate(localVarsNameToId,globalVarsNameToId,state,sh); 
    stringstream str;
    str << value;
    stringstream createOut;
    createOut << s.substr(0,firstPos+1) << str.str().c_str () << s.substr(lastPos);
    
    return createOut.str ();;
  }
  
  return s;
}

string XMLLoader::getResets (VarState& varS,  map<string,string> &localClocksNameToId, map<string,string> &localVars  ,string& update)
{
    ostringstream theStream;
    boost::regex spaces ("\\s*");
    string updateNoSpace = boost::regex_replace (update,spaces,"");
    string varNames ("[a-zA-Z][+\\-\\*\\\\%\\[\\]\\(\\)\\_\\.a-zA-Z0-9]*");
    string number ("0");
    boost::regex r (varNames+":="+number);
    boost::sregex_token_iterator it (updateNoSpace.begin (),updateNoSpace.end (),r);
    boost::sregex_token_iterator end;
    while (it!=end)
    {
        ostringstream buf;
        buf << (*it);
        
	string var = boost::regex_replace (buf.str(),boost::regex(":=0"),"");
        string id;
	var = removeInnerArith (var,  localVars, systemVariablesNameToId, varS);
        if ((id= localClocksNameToId[var]) != "")
            theStream << "," << id <<",";
        else if ((id=systemClocksNameToId[var]) != "")
            theStream << "," << id <<",";
        it++;
    }
    return theStream.str ();
}

list<dc_t*>* XMLLoader::CreateConstraints (string& str, ProcessInfoKeeper* procInfo, VarState varState)
{
    list<dc_t*>* theList = new list<dc_t*> ();
    istringstream instream (str);
    string buf;
    while (instream >> buf)
    {
        if (buf != "1")  // Hvad gør det her?
        {
	  string b = removeInnerArith (buf,procInfo->localVariablesNameToId, systemVariablesNameToId,varState);
	  dc_t* buffer = modifyConstraint (procInfo->localClocksNameToId, systemClocksNameToId, procInfo->localVariablesNameToId, systemVariablesNameToId, varState, b, zero);
	  
            if (buffer!=0)
            {
                if (buffer->getOp () == eq)
                {
                    dc_t* buf1 = new dc_t (buffer->getY (),buffer->getX(),leq,buffer->getBound ()*-1);
                    dc_t* buf2 = new dc_t (buffer->getX (),buffer->getY(),leq,buffer->getBound ());
                    delete buffer;
                    theList->push_front (buf1);
                    theList->push_front (buf2);
                }
                else
                    theList->push_front (buffer);
            }
        }
    }
    return theList;
}

bool XMLLoader::parseDocument (string& documentName, xmlDocPtr* doc2)
{
    (*doc2) = doc = xmlParseFile(documentName.c_str ());
    if (doc==0)
        return false;
    else
    {
        bool res =  parseTrace (xmlDocGetRootElement (doc));
        //xmlFreeDoc (doc);
        return res;
    }
}
bool XMLLoader::parseTrace (xmlNodePtr node)
{
    if (xmlStrcmp (node->name,(const xmlChar*) "trace"))
    {
        cerr << "It appears this is not a valid trace" << endl;
        return false;
    }

    else
    {
        xmlChar* prop = xmlGetProp (node,(const xmlChar*)"initial_node");
        initNode = string ((const char*) prop);
        xmlFree (prop);
        if (xmlStrcmp (prop = xmlGetProp (node,(const xmlChar*)"trace_options"),(const xmlChar*)"symbolic"))
        {
	  cerr << "Incompatible trace" << endl;
	  xmlFree(prop);
	  return false;
        }
        xmlFree(prop);
        bool isTrue = true;
        xmlNodePtr cur = node->xmlChildrenNode;
        while (cur!=0 && isTrue)
        {
            if (!xmlStrcmp (cur->name, (const xmlChar*)"system"))
            {
                isTrue = parseSystem (cur);
            }
            else if (!xmlStrcmp (cur->name, (const xmlChar*)"location_vector"))
            {
                isTrue = parseLocationVector (cur);
            }
            else if (!xmlStrcmp (cur->name, (const xmlChar*)"variable_vector"))
            {
                isTrue  = parseVariableVector (cur);
            }
            else if (!xmlStrcmp (cur->name, (const xmlChar*)"dbm_instance"))
            {
                isTrue  = parseDBM (cur);
            }
            else if (!xmlStrcmp (cur->name, (const xmlChar*)"node"))
            {
                isTrue  = parseNode (cur);
            }
            else if (!xmlStrcmp (cur->name, (const xmlChar*)"transition"))
            {
                isTrue = parseTransition (cur);
            }
            else
            {
                cerr  << "Unrecognized XML Element in trace," << (const char*)cur->name << endl;
            }
            cur = cur->next;
        }
        return true;
    }
}

bool XMLLoader::parseProcessClock (xmlNodePtr node, ProcessInfoKeeper* procInfo )
{
    xmlChar* c = xmlGetProp (node,(xmlChar*)"name");
    xmlChar* id = xmlGetProp (node,(xmlChar*)"id");
    if (c==0 || id==0)
    {
        freeMemoryIfPossible(id);
        freeMemoryIfPossible(c);
        return false;
    }

    string strName= string ((char*)c);
    string strId = string ((char*)id);
    clocks.push_back(strId);
    procInfo->localClocksNameToId[strName] = strId;
    int i = (strId == zero) ? (0) : nextClockId;
    //cout << "Adding clock" << strId << endl;
    clocksIndex[strId] = i;
    if (i!=0)
      nextClockId++;
    xmlFree(c);
    xmlFree(id);
    return true;
}

bool XMLLoader::parseSystem (xmlNodePtr node)
{
    xmlNodePtr cur = node->xmlChildrenNode;
    while (cur!=0)
    {
        if (!xmlStrcmp (cur-> name, (const xmlChar*)"clock"))
        {
            parseClock (cur);
        }
        else if (!xmlStrcmp (cur->name, (const xmlChar*)"variable"))
        {
            parseVariable (cur);
        }
        else if (!xmlStrcmp (cur->name, (const xmlChar*)"process"))
        {
            parseProcess (cur);
        }
        else
        {
	  cerr  << "Unrecognized XML Element in system," << (const char*)cur->name << endl;
        }
        cur = cur->next;
    }
    return true;
}

bool XMLLoader::parseClock (xmlNodePtr node)
{
    
    xmlChar* c = xmlGetProp (node,(const xmlChar*)"name");
    xmlChar* id = xmlGetProp (node,(const xmlChar*)"id");
    if (c==0 || id==0)
    {
        return false;
    }
    
    string strName= string ((const char*)c);
    string strId = string ((const char*)id);
    
    clocks.push_back(strId);
    systemClocksNameToId[strName] = strId;
    int i = (strId == zero) ? (0) : nextClockId;
    //cout << "Adding clock" << strId << endl;
    clocksIndex[strId] = i;
    if (i!=0)
      nextClockId++;
    return true;
}

bool XMLLoader::parseProcess (xmlNodePtr node)
{
    ProcessInfoKeeper* procInfo = new ProcessInfoKeeper;
    procInfoKeepers.push_back(procInfo);

    xmlNodePtr cur = node->xmlChildrenNode;
    while (cur!=0)
    {
        if (!xmlStrcmp (cur-> name, (const xmlChar*)"clock"))
        {
            parseProcessClock (cur, procInfo);
        }

        else if (!xmlStrcmp (cur->name, (const xmlChar*)"variable"))
        {
            parseProcessVariable (cur,procInfo);
        }
        else if (!xmlStrcmp (cur->name, (const xmlChar*)"edge"))
        {
            parseEdge (cur,procInfo);
        }
        else if (!xmlStrcmp (cur->name, (const xmlChar*)"location"))
        {
            parseLocation (cur,procInfo);
        }
        else
        {
	  cerr  << "Unrecognized XML Element in system," << (const char*)cur->name << endl;
        }
        cur = cur->next;
    }
    return true;
}


bool XMLLoader::parseProcessVariable (xmlNodePtr node, ProcessInfoKeeper* procInfo)
{
    bool returnval = false;
    xmlChar* id = xmlGetProp (node,(xmlChar*) "id");
    xmlChar* name = xmlGetProp (node,(xmlChar*) "name");
    if (id!=0 && name!=0)
    {
        string strId ((char*) id);
        string strName ((char*)name);
        procInfo->localVariablesNameToId[strName] = strId;
        returnval = true;
    }
    freeMemoryIfPossible(id);
    freeMemoryIfPossible(name);
    return returnval;
}

bool XMLLoader::parseVariable (xmlNodePtr node)
{
    xmlChar* id = xmlGetProp (node,(xmlChar*) "id");
    xmlChar* name = xmlGetProp (node,(xmlChar*) "name");
    if (id==0 || name==0)
    {
        freeMemoryIfPossible(id);
        freeMemoryIfPossible(name);
        return false;
    }
    string strId ((char*) id);
    string strName ((char*)name);
    systemVariablesNameToId[strName] = strId;
    return true;
}
bool XMLLoader::parseLocation (xmlNodePtr node, ProcessInfoKeeper* procInfo)
{
    xmlChar* id = xmlGetProp (node,(xmlChar*) "id");
    xmlChar* name = xmlGetProp (node,(xmlChar*) "name");
    if (id==0 || name==0)
    {
        freeMemoryIfPossible(id);
        freeMemoryIfPossible(name);
        return false;
    }
    string strId ((char*)id);
    string strName ((char*)name);
    xmlFree(id);
    xmlFree(name);
    xmlChar* inv = xmlNodeGetContent(node);
    string invariant ="";
    if (inv!=0)
    {
        invariant = string ((char*)inv);
        xmlFree (inv);
    }
    LocationInfoKeeper loc;
    loc.parentProcess = procInfo;
    loc.invariant = invariant;
    loc.name = strName;
    locations[strId] = loc;
    return true;
}

bool XMLLoader::parseEdge (xmlNodePtr node, ProcessInfoKeeper* procInfo)
{
    xmlNodePtr cur = node->xmlChildrenNode;
    EdgeInfoKeeper keeper;
    keeper.parentProcess = procInfo;
    while (cur != 0)
    {
        if (!xmlStrcmp (cur-> name, (const xmlChar*)"guard"))
        {
            parseGuard (cur,keeper);
        } 
        else if (!xmlStrcmp (cur-> name, (const xmlChar*)"sync"))
        {
            parseSync (cur,keeper);
        }
        else if (!xmlStrcmp (cur-> name, (const xmlChar*)"update"))
        {
            parseUpdate (cur,keeper);
        }
        else
        {
            cerr  << "Unrecognized XML Element in Edge," << (const char*)cur->name << endl;
        }
        cur = cur->next;
    }

    xmlChar* id = xmlGetProp (node,(const xmlChar*) "id");
    if (id!=0)
    {
        string strId ((char*)id);
        edges[strId] = keeper;
        xmlFree(id);
    }
    return true;
}


bool XMLLoader::parseGuard (xmlNodePtr node,EdgeInfoKeeper& keep)
{
    xmlChar* g= xmlNodeGetContent(node);
    string guard ="";
    if (g!=0)
    {
        guard = string ((const char*)g);
        xmlFree(g);
    }
    keep.guard = guard;
    return true;
}
bool XMLLoader::parseSync (xmlNodePtr node, EdgeInfoKeeper& keep)  // What is this used for?
{
    xmlChar* g= xmlNodeGetContent(node);
    string sync ="";
    if (g!=0)
    {

        sync = string ((const char*)g);
        xmlFree(g);
    }

    keep.sync = sync;
    return true;
}

bool XMLLoader::parseUpdate (xmlNodePtr node, EdgeInfoKeeper& keep)
{
    xmlChar* g= xmlNodeGetContent (node);
    string update ="";
    if (g!=0)
    {
        update = string ((const char*)g);
        xmlFree(g);
    }
    keep.update = update;
    return true;
}

void XMLLoader::freeMemoryIfPossible(xmlChar* ptr)
{
    if (ptr != 0)
        xmlFree(ptr);
}

bool XMLLoader::parseLocationVector (xmlNodePtr node)
{
    xmlChar* id = xmlGetProp (node,(xmlChar*) "id");
    xmlChar* locs = xmlGetProp (node,(xmlChar*) "locations");
    if (id==0 || locs==0)
    {
        freeMemoryIfPossible(id);
        freeMemoryIfPossible(locs);
        return false;
    }
    string strId ((char*)id);
    string strLocs ((char*)locs);
    xmlFree(id);
    xmlFree(locs);
    locationVectors[strId] = strLocs;
    return true;
}

bool XMLLoader::parseDBM (xmlNodePtr node)
{
  Bound inf;
  //First construct a keep;
  DBMKeeper keep;
  xmlChar* id  = xmlGetProp (node,(xmlChar*) "id");
  string strId ((char*)id);
  keep.id = strId;
  xmlFree (id);
  xmlNodePtr cur = node->xmlChildrenNode;
  while (cur!=0)
    {
      xmlChar* firstVar  = xmlGetProp (cur,(xmlChar*) "clock1");
      xmlChar* secondVar = xmlGetProp (cur,(xmlChar*) "clock2");
      string firstStrVar ((char*) firstVar);
      string secondStrVar ((char*) secondVar);
      xmlFree (firstVar);
      xmlFree (secondVar);
      xmlChar* cmp = xmlGetProp (cur,(xmlChar*) "comp");
      bool strict = !xmlStrcmp (cmp,(const xmlChar*)"<");
      xmlFree (cmp);
      
      xmlChar* val = xmlGetProp (cur,(xmlChar*) "bound");
      string str ((char*)val);
      xmlFree (val);
      
      BoundKeeper bKeeper;
      bKeeper.first = firstStrVar;
      bKeeper.second = secondStrVar;
      if (str == string ("inf")) 
	bKeeper.bound = inf;
      else 
	bKeeper.bound = Bound (mpq_class (str),strict);
      
      keep.bounds.push_back (bKeeper);
      cur = cur->next;
    }
  dbmKeeps.push_back (keep);
  return true;
}

bool XMLLoader::parseNode (xmlNodePtr node)
{
    bool returnval = false;
    xmlChar* id = xmlGetProp (node,(const xmlChar*) "id");
    xmlChar* locs = xmlGetProp (node,(const xmlChar*) "location_vector");
    xmlChar* vvec = xmlGetProp (node,(const xmlChar*) "variable_vector");
    xmlChar* dbm = xmlGetProp (node,(const xmlChar*) "dbm_instance");
    if (id!=0 && locs!=0 && vvec!=0&&dbm!=0)
    {
        string strId ((const char*)id);
        string strLocs ((const char*)locs);
        string strDbm  ((const char*)dbm);
        string strVvec  ((const char*)vvec);
        NodeKeeper keep;
        keep.dbm = strDbm;
        keep.locationVector = strLocs;
        keep.variableVector = strVvec;
        nodes[strId] = keep;

        returnval = true;
    }
    freeMemoryIfPossible(id);
    freeMemoryIfPossible(locs);
    freeMemoryIfPossible(vvec);
    freeMemoryIfPossible(dbm);
    return returnval;
}

bool XMLLoader::parseTransition (xmlNodePtr node)
{
    bool returnval = false;
    xmlChar* from = xmlGetProp (node,(const xmlChar*) "from");
    xmlChar* to = xmlGetProp (node,(const xmlChar*) "to");
    xmlChar* edges = xmlGetProp (node,(const xmlChar*) "edges");
    if (from != 0 && to != 0 && edges != 0)
    {

        string strFrom ((const char*) from);
        string strTo ((const char*) to);
        string strEdges ((const char*) edges);
        // we will order the transitions by which node they are from
        TransitionKeeper* keep = new TransitionKeeper;
        keep->from = strFrom;
        keep->to = strTo;
        keep->edges = strEdges;
        transitions[strFrom] = keep;
        returnval = true;
    }
    freeMemoryIfPossible(from);
    freeMemoryIfPossible(to);
    freeMemoryIfPossible(edges);
    return returnval;
}
bool XMLLoader::parseVariableVector (xmlNodePtr node)
{
    xmlChar* id = xmlGetProp (node,(const xmlChar*) "id");
    VarState vState;
    if (id!=0)
    {
        string strId ((const char*)id);
        xmlNodePtr cur  = node->xmlChildrenNode;
        while (cur!=0)
        {
            if (!xmlStrcmp (cur->name,(const xmlChar*)"variable_state"))
            {
                parseVariableState (cur,vState);
                cur = cur->next;
            }
            else
            {
                cerr  << "Unrecognized XML Element in VariableVector," << (const char*)cur->name << endl;
                return false;
            }
        }
        variableStates[strId] = vState;
        xmlFree(id);
        return true;
    }

    return false;
}
bool XMLLoader::parseVariableState (xmlNodePtr node,VarState& varState)
{
    bool returnval = false;
    xmlChar* value = xmlGetProp (node,(const xmlChar*) "value");
    xmlChar* id = xmlGetProp (node,(const xmlChar*) "variable");
    if (value!=0 && id !=0)
    {
        stringstream str ((const char*)value);
        int val;
        str >> val;
        string strId ((const char*)id);
        //We are currently assuming we have put the variables names into
        //the list of variables
        varState[strId] = val;
        returnval = true;
    }
    freeMemoryIfPossible(value);
    freeMemoryIfPossible(id);
    return returnval;
}

Node* XMLLoader::createNode (NodeKeeper& nodeKeeper)
{
    istringstream locStr (locationVectors[nodeKeeper.locationVector]);
    string location;
    list<dc_t*>* constraints = new list<dc_t*> ();

    boost::regex spaces ("\\s*");
    while (locStr >> location)
    {
        string invariant =  locations[location].invariant;
        ProcessInfoKeeper* procInfo = locations[location].parentProcess;
        
        invariant = boost::regex_replace (invariant,spaces,"");
        boost::regex reg ("&&");
        invariant = boost::regex_replace (invariant,reg," ");
        constraints->splice(constraints->end(), *(CreateConstraints(invariant, procInfo, variableStates[nodeKeeper.variableVector])));
    }
   
    string db = boost::regex_replace (nodeKeeper.dbm,spaces,"");
    
    
    Node *n = 0;//loopDetector[nodeKeeper.locationVector+nodeKeeper.variableVector];
    DBM dbm = dbms[db];
    if (n!=0)
      {
	
	DBM ndbm = n->getDBM ();
	if (dbm.subset (ndbm))
	  {
	    
	    n->setDBM (dbm);
	    
	    n->setLoop ();
	    return n;
	  }
	 
      }
	
    
    n = new Node (constraints); 
    
    n->setDBM (dbms[db]);
    loopDetector[nodeKeeper.locationVector+nodeKeeper.variableVector] = n;
    return n;
}

Transition* XMLLoader::createTransition (TransitionKeeper& transKeeper)
{
  
    VarState state = (variableStates[nodes[transKeeper.from].variableVector]);
    list<dc_t*>* constraints = new list<dc_t*> ();

    istringstream edgStr (transKeeper.edges);
    string edge, reset="";
    while (edgStr >> edge)
    {
        string guard = edges[edge].guard;
        ProcessInfoKeeper* procInfo = edges[edge].parentProcess;
        boost::regex spaces ("\\s*");
        guard = boost::regex_replace (guard,spaces,"");
        boost::regex reg ("&&");
        guard = boost::regex_replace (guard,reg," ");
        constraints->splice(constraints->end(), *(CreateConstraints (guard, procInfo, state)));

        reset += getResets (state,procInfo->localClocksNameToId,procInfo->localVariablesNameToId,edges[edge].update);
    }
   
    return new Transition(reset,constraints,transKeeper.from,clocksIndex);
}

LTA::Lta* XMLLoader::createLTA ()
{
  map<string, Node*> nodesAdded;
    NodeKeeper xmlNode = nodes[initNode];
    Node* ltaNode = createNode (xmlNode);
    LTA::Lta* lta = new LTA::Lta (ltaNode,new vector<string> (clocks));
   
    TransitionKeeper* xmlTransition = transitions[initNode];
    Transition* ltaTransition;

    string nextNodeId;
    while(xmlTransition != 0)
    {
        ltaTransition = createTransition(*xmlTransition);
        nextNodeId = xmlTransition->to;
        xmlNode = nodes[nextNodeId];
        
	ltaNode = nodesAdded[nextNodeId];
	if (ltaNode!=0 && transitions[nextNodeId]!=0) {
	  ltaNode->setLoop ();
	  ltaTransition ->setLoop ();
	  lta->add (ltaTransition,ltaNode);
	  break;
	}
	else {
	  ltaNode= createNode (xmlNode);
	  if (ltaNode->isLoopLoc()) 
	    ltaTransition ->setLoop ();
	  nodesAdded[nextNodeId] = ltaNode;
	  lta->add (ltaTransition,ltaNode);
	  xmlTransition = transitions[nextNodeId];
	}
    }
    return lta;
}

void XMLLoader::createDBMS () {
  
  list<DBMKeeper>::iterator iter;
 
  for (iter = dbmKeeps.begin (); iter!=dbmKeeps.end ();iter++) {
    DBM d (clocks.size ());
 
    vector<BoundKeeper>::iterator bIt;
    for (bIt =(*iter).bounds.begin (); bIt != (*iter).bounds.end ();bIt++) {
      d.setConstraint (clocksIndex[bIt->first],clocksIndex[bIt->second],bIt->bound);
      stringstream stream;
      stream << bIt->bound.getBound ();
      int i;
      stream >> i;
     
    }
    //d.close(true);
    dbms[(*iter).id] = d;
  }
}


LTA::Lta* XMLLoader::generateLTA (string& path,xmlDocPtr* ptr,map<string, int>& clockIndex)
{
    if (!parseDocument (path,ptr))
    {
        return 0;
    }
    
   
    //Now we have everything set u.
    //We can now generate the LTA from these information::
    clockIndex = clocksIndex; 
    createDBMS ();
    return createLTA ();
}



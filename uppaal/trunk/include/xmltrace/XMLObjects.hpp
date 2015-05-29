
#ifndef _XMLOBJECTS__
#define _XMLOBJECTS__
#include "system/Variables.h"
#include "base/inttypes.h"
#include "system/Zone.h"
#include "xmltrace/Util.hpp"
#include <list>
#include <string>
#include <iostream>
#include <vector>
using namespace std;
using std::list;
using std::vector;
using std::string;
//FIle containing objects with the sole purpose of representing the data to be// outputted to the XML-FIle

class Process;
class Edge;
class Location;

class VariableVector;
//Clocks perhaps also

class XMLLocationVector {
private:
  string id;
  list<string> locations;
public:
  XMLLocationVector (string Id) : id (Id) {}
  
  void addLocation (string s) {locations.push_back (s);}
  list<string>* getLocations (); 
  string getId ();
};

class XMLVariable {
private:
  const Variable* var;
  string id;
public:
  XMLVariable (const Variable* v,string Id) :id (Id) {
    var = v;
  }
 
  
  string getName (){
    return var->getName ();
  }
  
  string getId () {
    return id;
  }
  
};

class XMLVarState {
private:
  string id;
  double value;
public:
  XMLVarState (string Id, double val) : id(Id) ,value (val) {}
  
  double getValue () {return value;}
  string getId (){return id;}
};

class XMLVarVector {
private:
  const VariableVector* vvector;
  vector<XMLVarState*> theList;
  string id;
public:
  XMLVarVector (const VariableVector* vvec, string Id) : id (Id) {
    vvector = vvec;
  }

  
  void add (XMLVarState* v) {
    theList.push_back (v);
  }

  uint32_t getNrOfVars ();
  string getIdOfVar (int);
  double getValOfVar (int);
  string getId ();
};
  

class XMLEdge {
private:
  const Edge *edge;

  string id;
  string from;
  string to;
  const string guard;
  const string sync;
  const string update;
public:
  XMLEdge (const Edge* e, string Id, string From, string To, const string g,
	   const string s, const string u)  : 
    id(Id), from (From), to (To), guard(g),sync(s),update(u) {}  
  string getId ();
  string getTo ();
  string getFrom ();
  string getGuard ();
  string getSync ();
  string getUpdate ();
};
    
class XMLLocation {
private:
  const Location* loc;
  string id;
  bool initial;
  string invariant;
public:
  XMLLocation (const Location* l,string Id,bool init,string inv ) : id (Id),invariant(inv) {
    loc = l;
    initial = init;
  }  
    
  string getName ();
  string getId ();
  string getInvariant ();
  bool isUrgent ();
  bool isCommitted ();
  bool isInitial ();

};

class XMLProcess {
private:
  const Process* process;
  list<XMLEdge*>* edges;
  list<XMLLocation*>* locations;
  string id;
 
public:
  XMLProcess (const Process* p,string Id) : id (Id){
    process = p;
    edges = new list<XMLEdge*> ();
    locations = new list<XMLLocation*> ();
  }
    
  ~XMLProcess () {
     
    list<XMLEdge*> ::iterator it;
    for (it=edges->begin (); it!=edges->end ();it++) {
      delete (*it);
    }

    delete edges;

    list<XMLLocation*> ::iterator ite;
    for (ite=locations->begin (); ite!=locations->end ();ite++) {
      delete (*ite);
    }

    delete locations;
  }
    
  void addEdge (XMLEdge* e) {
   
    edges->push_front (e);
  }
  void addLocation (XMLLocation* l) {
    locations->push_front (l);
  }
  list<XMLEdge*>& getEdges ();
  list<XMLLocation*>& getLocations ();
  string getId ();
  string getName ();
};


class XMLZone {
  const Zone* zone;
  string id;
public:
  XMLZone (string Id,const Zone* z) : id (Id) {zone = z;} 

  string getRep ();
  string getId ();
};

class XMLNode {
private:
  string id;
  string locationVec;
  string varVec;
  list<string>* dbms;
public:
  XMLNode (string Id, string loc, string vVec,list<string>* d) : id(Id),locationVec (loc), varVec (vVec) {
    dbms= d;
  }

  string getId () {return id;};
  string getLocationVector () {return locationVec;}
  string getVarVector () {return varVec;}
  list<string>* getDbms () {return dbms; }
};

class XMLTransition {
private:
  string to;
  string from;
  list<string>* edges;
public:
  XMLTransition (string From, string To, list<string>* edg) 
    : to(To) ,from(From) {
    edges = edg;
  }

  string getTo () {return to;}
  string getFrom () {return from;}
  list<string>* getEdges () {return edges;}
  
};

class XMLClock {
private:
  string id;
  string name;
public:
  XMLClock (string Id, string Name) : id (Id),name (Name) {}
  string getId () {return id;}
  string getName () {return name;}
};

class XMLSystem {
public:
  const list<XMLVariable*>* variables;
  const list<XMLClock*>* clocks;
  const list<XMLProcess*>* processes;
  XMLSystem (list<XMLVariable*>* vars, list<XMLClock*>* Clocks, list<XMLProcess*>* p) {
    variables = vars;
    clocks = Clocks;
    processes = p;
  }
  ~XMLSystem () {
    //The objects are mine...therefore i deallocate them 
    deinit (const_cast<list<XMLVariable*>*>(variables));
    deinit (const_cast<list<XMLClock*>*>(clocks));
    deinit (const_cast<list<XMLProcess*>*>(processes)); 
  }
  
};

class XMLTrace {
public:
 
  const list<XMLVarVector*>* variableVectors;
  const list<XMLTransition*>* transitions;
  const list<XMLNode*>* nodes;
  const list<XMLZone*>* zones;
  const list<XMLLocationVector*>* locationVectors;
  const string initialNode;
  const XMLSystem* system;
  
  XMLTrace (string node,
	    list<XMLVarVector*>* v,
	    list<XMLTransition*>* t,
	    list<XMLNode*>* n,
	    list<XMLZone*>* z,
	    list<XMLLocationVector*>* lv,
	    XMLSystem* sys) : initialNode (node){
    nodes = n;
    variableVectors = v;
    transitions = t; 
    zones =z; 
    locationVectors=lv;
    system = sys;
  }
  
  ~XMLTrace () {
    deinit (const_cast<list<XMLZone*>*>(zones));
    deinit (const_cast<list<XMLVarVector*>*>(variableVectors));
    deinit (const_cast<list<XMLTransition*>*>(transitions));
    
    deinit (const_cast<list<XMLLocationVector*>*>(locationVectors));
    delete system;
  }

  string getInitialNode () {
    return initialNode;

  }

  
  

};
    

#endif  

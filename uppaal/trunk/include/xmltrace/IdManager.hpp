#include "base/inttypes.h"
#include "base/pointer.h"
#include "verifier/Path.h"
#include "system/Edge.h"
#include "system/Process.h"
#include "system/Location.h"
#include "system/Zone.h"
#include "system/Variables.h"
#include "system/SymbolicState.h"
#include "system/System.h"
#include "xmltrace/Stores.hpp"
#include "xmltrace/XMLObjects.hpp"
#include "xmltrace/Util.hpp"


#include <list>
#include <string>
#include <map>

using std::map;
using std::list;
using std::string;

struct ProcessObject{
  const Process* process;
  EdgeStore edgeStore;
  LocationStore locationStore;
};


class NameManager {
private:
  map<string,list<string>*> theNameMap; 
  map<string,map<string,int>*> theIntMap;
  map<string,string> idMap;
  bool getVal (string,string,int&);
public:
  void registerName (string registerTo, string toRegister);
  string getName ( string context,  string id) ;
  void registerInt (string registerTo, string toRegister,int);
  
};


typedef DeinitStore<ProcessObject> ProcessObjectStore;
typedef Store<XMLProcess> XMLProcessStore;
typedef Store<const Location*> LocationVectorStore;
typedef Store<const Variable> VarStore;


 
class IdManager {
private:
  NameManager nameMan;
  int nextNodeId;
  int nextTransId;
  string lastNode;
  
  ZoneStore zoneStore;
  VariableVectorStore variableVectorStore;
  ProcessObjectStore processObjectStore;
  VarStore varStore;
  LocationVectorStore locationVectorStore;
  XMLProcessStore xmlProcessesStore;
  

  SymbolicState* state;
  System* sys;
  
  bool firstNode;
  string initNode;
  
  list<XMLProcess*>* listOfXMLProcess;
  list<XMLVariable*>* listOfXMLVars;
  list<XMLVarVector*>* listOfXMLVarVectors;
  list<XMLLocationVector*>* listOfXMLLocationVectors;
  list<XMLZone*>* listOfXMLZones; 
  list<XMLNode*>* listOfXMLNodes;
  list<XMLTransition*>*  listOfXMLTransitions;
  list<XMLClock*>* listOfXMLClocks;
  
  virtual uint32_t getHash (const Process*);
  virtual uint32_t getHash (const Edge*);
  virtual uint32_t getHash (const Location*);
  virtual uint32_t getHash (const Zone*);
  virtual uint32_t getHash (const VariableVector*);
  virtual uint32_t getHash (const Variable*);
  virtual string getVarId (int);

public:
  
  IdManager ()  : nextNodeId(0), nextTransId (0),lastNode (""){
    firstNode = true ;
    initNode = "";
    listOfXMLProcess = new list<XMLProcess*> ();
    listOfXMLVars = new list<XMLVariable*> ();
    listOfXMLVarVectors = new list<XMLVarVector*> ();
    listOfXMLLocationVectors = new list<XMLLocationVector*>();
    listOfXMLZones = new list<XMLZone*> ();
    listOfXMLNodes = new list<XMLNode*> ();
    listOfXMLTransitions = new list<XMLTransition*> ();
    listOfXMLClocks = new list<XMLClock*> ();
    assert (listOfXMLVarVectors);
  }
  
  ~IdManager () {
    //cout << "Deinitint" << endl;
    //cout.flush (); 
    //deinit lists
      deinit (listOfXMLProcess);
      deinit (listOfXMLVars);
      deinit (listOfXMLVarVectors);
      deinit (listOfXMLLocationVectors);
      deinit (listOfXMLZones);
      deinit (listOfXMLNodes);
      deinit (listOfXMLTransitions);
      deinit (listOfXMLClocks);
  }

  void setSystem (System*);
  
  void addState (SymbolicState*, ConstEdgeArray&);
  
  virtual string  getId (const Process*);
  virtual string  getId (const Edge*);
  virtual string  getId (const Location*);
  virtual string  getId (const Zone*);
  virtual string  getId (const VariableVector*);
  virtual string getId (const Variable*);
  
  virtual string  getIdLocVec (uint32_t);

 
  list<XMLProcess*>* xmlProcesses () {
    list<XMLProcess*>* buf = listOfXMLProcess;
    listOfXMLProcess = 0;
    return buf;
  }
  
  
  list<XMLVarVector*>* xmlVariableVectors () {
    assert (listOfXMLVarVectors);
    list<XMLVarVector*>* buf = listOfXMLVarVectors;
    listOfXMLVarVectors = 0;
    assert(buf);
    return buf;
  }
  
  list<XMLVariable*>* xmlVariables () {
    list<XMLVariable*>* buf = listOfXMLVars;
    listOfXMLVars = 0;
    return buf;
  }
  list<XMLLocationVector*>* xmlLocationVectors () {
    list<XMLLocationVector*>* buf = listOfXMLLocationVectors;
    listOfXMLLocationVectors = 0;
    return buf;
  }
  
  list<XMLZone*>* xmlZones () {
    list<XMLZone*>* buf = listOfXMLZones;
    listOfXMLZones = 0;
    return buf;
  }
  
  
  list<XMLNode*>* xmlNodes () {
    //cout << "Getting nodes" << endl;
    //cout.flush ();
    assert (listOfXMLNodes);
    list<XMLNode*>* buf = listOfXMLNodes;
    listOfXMLNodes = 0;
    assert (buf);
    return buf;
  }

    
  list<XMLTransition*>* xmlTransitions () {
    list<XMLTransition*>* buf = listOfXMLTransitions;
    listOfXMLTransitions = 0;
    return buf;
  }

  
  list<XMLClock*>* xmlClocks () {
    list<XMLClock*>* buf = listOfXMLClocks;
    listOfXMLClocks = 0;
    return buf;
  }

  void add (const Process*);
  void add (const Edge*);
  void add (const Location*);
  void add (const Zone*);
  void add (const VariableVector*);
  void add (const Variable* );

  string getInitNode () {
    return initNode;
  }
};

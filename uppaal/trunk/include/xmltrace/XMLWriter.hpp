#include <stdio.h>
#include <string>
#include <iostream>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include "base/inttypes.h"
#include "xmltrace/XMLObjects.hpp"

using namespace std;

namespace XMLWrite {

  typedef XMLTrace  Trace;
  typedef XMLSystem  System;
  typedef XMLLocation  Location;
  typedef XMLLocationVector LocationVector;
  typedef XMLVarVector  VariableVector;
  typedef XMLEdge  Edge;
  typedef XMLZone  DBMInstance;
  typedef XMLProcess  Process;
  typedef XMLTransition  Transition;
  typedef XMLNode  Node;
  typedef XMLClock  Clock;
  typedef XMLVariable Variable;
#define ENCODING "UTF8"

/* This class is tightly related to the XML document 
 * type definition as most of the methods here represent
 * elements in the XML dtd. 
 *
 * TODO: Make the class extendable and add WriteData 
 * and allow users to use the extra data fields by implementing 
 * simple methods (perhaps by using some DEFINE and IFDEF combination?)
 */

  class TraceWriter 
  {
    xmlTextWriterPtr writer;
    int status;    
  public:
    void WriteTrace(Trace*, string*);
    void WriteTrace(Trace*);
    void WriteProcess(Process*);
    void WriteEdge(Edge*);
    void WriteLocation(Location*);
    void WriteGuard(string*);
    void WriteSync(string*);
    void WriteUpdate(string*);
    void WriteLocationVector(LocationVector*);
    void WriteNode(Node*);
    void WriteTransition(Transition*);
    void WriteVariableVector(VariableVector*);
    void WriteSystem(System*);
    void WriteDBM(DBMInstance*);
    
  private:
  void InitWriter(string*);
    void WriteClock(Clock*);
    void WriteVariable(Variable*);
  };
  
  const  char* Double2String(double);
}

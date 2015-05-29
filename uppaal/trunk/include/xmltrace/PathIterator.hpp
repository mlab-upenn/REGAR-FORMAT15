#ifndef _PATHITERATOR__
#include "base/inttypes.h"
#include "base/pointer.h"
#include "verifier/Path.h"
#include "system/Edge.h"
#include "system/Process.h"
#include "system/Location.h"
#include "system/VariableVector.h"
#include "system/Variables.h"
#include "system/Zone.h"
#include "xmltrace/Stores.hpp"
#include "xmltrace/IdManager.hpp"
#include "xmltrace/XMLObjects.hpp"
//Class to observe the iterations over a Path..
//The idea is that if people wants to do different things depending than what //we are doing they can reimlement these functions :-D



class PathIterator {
private:
  IdManager manager;
  void iterateFederations (const Federation* fed);
protected:
  
  //LTA construction functions
  //We will miss these for the momoment...
  //We will  need to define how the objects looks like first :-D 
  /*  virtual void newNode ();
  virtual void newTransition ();
  */
  //UPPAAL observation functions.
  virtual void newVariable (const Variable*);
  virtual void newEdge (const Edge*);
  virtual void newLocation (const Location*);
  virtual void newProcess (const Process*);
  virtual void newVariableVector (const VariableVector*);

  //Needs functions to handle Federations, 
public:
  void iterate (base::Pointer<Path>&, base::Pointer<Path>&);
  XMLTrace* constructTrace ();
  XMLSystem* constructSystem ();
};
  

  
#endif

#ifndef _TRACE__
#define _TRACE__

#include <vector>

#include "realDBM/dbm.hpp"
#include "lta/lta.hpp"
#include "main/Options.hpp"
using std::vector;

namespace smallzone {
  class Node;
  class Transition {
  private:
    DBM guard;
    vector<int> reset;
    Node* from;
    Node* to;
   
  public:
    Transition (DBM, vector<int>, Node*,Node*);
    ~Transition ();
    DBM& getGuard ();
    vector<int>& getReset ();
    Node* getTo ();
    
    Node* getFrom ();

  };
    
  class Node {
  private:
    DBM entry;
    DBM delay;
    Transition* trans;
    Transition* backTrans;

    VariableAssignment valEntry;
    VariableAssignment valDelay;
  public:
    Node (DBM,DBM);
    ~Node ();
    DBM& getEntry ();
    DBM& getDelay ();
    void setBackward (Transition*);
    void setForward (Transition*);
    Transition* getBackward ();
    Transition* getForward ();
    
    void setEntry ( VariableAssignment v);
    VariableAssignment getEntryV ();
    void setDelay ( VariableAssignment v);
    VariableAssignment getDelayV ();
  };
  
  class Trace {
  private: 
    Node* first;
    Node* last;
    string zero;
    string refClock;
     int states;
  public:
    Trace (LTA::Lta*,Options*);
    ~Trace () {if (first!=0) delete first;}
    Node* getFirst () {return first;}
    Node* getLast () {return last;}
    void add (DBM&, vector<int>,DBM&);
    string getRefClock ();
    int getStates () {return states;}
  };

}

#endif

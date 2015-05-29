

#ifndef _LIVETRACE__
#define _LIVETRACE__

#include <vector>

#include "rationalDBM/dbm.hpp"
using namespace newdbm;
using std::vector;

namespace liveness {

  typedef vector<int> IntAssignment;

  struct Op{
    DBM init;
    DBM guard;
    vector<int> reset;
  };

  class Node {
  private:
    DBM upZone;
    DBM entryZone;
    DBM guard;
    vector<int> reset;
    Node* next;
    Node* prev;
    VariableAssignment enter;
    VariableAssignment leave;
    
  public:
    Node (DBM,DBM,vector<int>);
    DBM getUpped ();
    DBM getEntry ();
    void setEntry (DBM);
    void setUp (DBM);
    DBM getGuard ();
    vector<int> getReset();
    void setNext (Node*);
    Node* getNext ();
    Node* getPrev ();
    void setPrev (Node*);
    void setEnter (VariableAssignment v) {enter = v;}
    void setLeave (VariableAssignment v) {leave = v;}
    VariableAssignment getEnter () {return enter;}
    VariableAssignment getLeave () {return leave;}
  };
  
  class LinSequence {
  private:
    bool deleteNodes;
    Node* first;
    Node* last;
    void calcEntryZones ();
  public:
    ~LinSequence ();
    LinSequence (VariableAssignment, vector<Op>& );
    LinSequence (vector<Op>&);
    void calcZones();
    void calcZones(VariableAssignment v);
    void findAssignments (VariableAssignment&);
    void splice (LinSequence* );
    void stabilize ();
    Node* getFirst ();
    Node* getLast ();
    
  };

    
}
#endif


#include <vector>
#include "liveness/liveTrace.hpp"

#include "rationalDBM/dbm.hpp"
#include <iostream>

using namespace newdbm;

using std::cout;
using std::endl;
using std::vector;

namespace liveness {

  DBM Node::getUpped () {
    return upZone;
  }

  DBM Node::getEntry () {
    return entryZone;
  }

  DBM Node::getGuard () {
    return guard;
  }

  vector<int> Node::getReset () {
    return reset;
  }

  void Node::setNext (Node* n) {
    next= n;
  }

  Node* Node::getNext () {
    return next;
  }

  Node* Node::getPrev () {
    return prev;
  }

  void Node::setPrev (Node* m) {
    prev = m;
  }

  void Node::setEntry (DBM e) {
    entryZone= e;
  }

  void Node::setUp (DBM e) {
    upZone = e;
  }

  Node::Node (DBM u,DBM g, vector<int> r) {
    upZone = u;
    guard = g;
    reset = r;
  }

  Node* LinSequence::getFirst () {return first;}
  Node* LinSequence::getLast () {return last;}

  VariableAssignment doReset (VariableAssignment v, vector<int> reset) {
    vector<int>::iterator it;
    for (it = reset.begin ();it!=reset.end ();it++) {
      v[*it] = 0;
    }

    return v;
  }

  void LinSequence::stabilize () {
    VariableAssignment var;
    Node* cur = first->getNext ();
    Node*prev = first;
    while (cur!=0) {
      var = prev->getEnter ();

      cur->setEnter(doReset (var,prev->getReset ()));
      if (cur == last)
	cur = 0;
      else{
	prev = cur;
	cur = cur->getNext ();

      }


    }
  }

  void LinSequence::calcZones () {
    Node* cur;
    Node* prev;

    int numOfVars = getFirst ()->getUpped ().numOfVars (); 

    DBM initZone = DBM::initZone (numOfVars); 
    cur = getFirst ();
    DBM guard = cur->getGuard ();

    cur->setEntry (initZone);
    DBM up = cur->getUpped ();
    cur->setUp (initZone.up().intersect (up).intersect (guard));
    cur = cur->getNext ();
    int i = 0;
    while (cur !=0)  {
      vector<int> res = cur->getPrev()->getReset ();
      DBM entry = cur->getPrev ()->getUpped().reset (res);
      DBM origUp = cur->getUpped ();
      cur->setEntry (entry);
      cur->setUp (entry.up().intersect (origUp));
      if (cur!=last)
	cur = cur->getNext ();
      else
	cur = 0;
    }
  }

  LinSequence::LinSequence (vector<Op>& ops) {
    Node* cur;
    Node* prev;
    for (int i=0;i<ops.size ();i++) {
      cur = new Node (ops[i].init,ops[i].guard,ops[i].reset);
      if (i==0) {
	first = cur;
      }
      else {
	cur->setPrev (prev);
	prev->setNext (cur);
      }

      prev = cur;
    }
    last = prev;
      //LinSequence (var,ops);
  }

  LinSequence::LinSequence (VariableAssignment v, vector<Op>& ops) {
    Node* prev;
    Node* cur;
   
    for (int i = 0;i< ops.size ();i++) {

      if (i == 0) {

	cur = new Node (DBM(v).up().intersect (ops[i].init),
			ops[i].guard,
			ops[i].reset);
	first = cur;
      }

      else {
	
	cur = new Node (prev->getUpped().
			intersect (ops[i-1].guard).
			reset (ops[i-1].reset).
			up ().
			intersect (ops[i].init)
			,ops[i].guard
			,ops[i].reset);
	prev -> setNext (cur);
	cur->setPrev (prev);
      }

      prev = cur;
    }

    last = prev;

  }


  void LinSequence::splice (LinSequence* other) {
  
    last->setNext (other->getFirst ()->getNext ());
    other->getFirst ()->getNext ()->setPrev (last);
    delete other->first;
    other->deleteNodes = false;
    }

  void LinSequence::calcEntryZones () {
    Node* cur = first->getNext ();
    while (cur!=0) {
      Node* prev = cur->getPrev();
      DBM upped = prev->getUpped ();
      vector<int> res = prev->getReset ();
      cur->setEntry (upped.reset(res));
      if (cur!= last) {
	cur = cur->getNext();
      }
      else {
	cur = 0;
      }
    }
  }


  void LinSequence::findAssignments (VariableAssignment& v) {
    calcEntryZones ();
    
   
    Node* cur = getLast ();
    Node* first = getFirst ();
    DBM dbm(v);
    cur->setLeave (v);
   
    DBM entry = cur->getEntry ();
    cur->setEnter (DBM(v).down().intersect (entry).findPoint ());
    
    cur = cur->getPrev ();
    while(cur!=0) {

      DBM entry = cur->getEntry ();
      DBM guard = cur->getGuard ();
      DBM upped = cur->getUpped ();
      vector<int> r = cur->getReset ();
      VariableAssignment var = cur->getNext ()->getEnter ();
      VariableAssignment leave = DBM(var).free (r).intersect (upped).intersect(guard).findPoint ();
    
      cur->setLeave (leave);
      
      if (cur!=first){
	VariableAssignment enter = DBM(leave).down().intersect(entry).findPoint ();
      cur->setEnter (enter);
    
      
    }

      if (cur == first)
	cur = 0;
      else
	cur = cur->getPrev ();

    }

  }


  LinSequence::~LinSequence () {
    if (deleteNodes) {
      Node* cur = first;
      while (cur != last) {
	Node* buf = cur;
	cur = cur->getNext ();
	delete buf;
      }

      delete last;
    }

  }

}

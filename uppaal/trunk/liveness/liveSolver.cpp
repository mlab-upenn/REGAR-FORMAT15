#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include "solver/interfaceStat.hpp"
#include "solver/interface.hpp"
#include "symbTrace/Location.hpp"
#include "symbTrace/Edge.hpp"
#include "symbTrace/lta.hpp"
#include "liveness/liveTrace.hpp"
#include "simpleLiveness.cpp"
#include "liveness/liveResult.hpp"


using std::vector;
using std::map;
//using std::cout;
using std::endl;
using std::istringstream;
using std::stringstream;
using namespace liveness;

void output (VariableAssignment var, map<string,int> index)
{
    map<string,int>::iterator it;
    for (it= index.begin ();it!=index.end ();it++)
    {
        cout << (*it).first << ": " <<var[(*it).second] << endl;
    }
}
vector<int> convert (string s, map<string,int> index)
{

    vector<int> buf;
    istringstream str (s);
    string buffer;
    while (std::getline(str, buffer, ','))
    {

        if (buffer!= "")
        {

            buf.push_back (index[buffer]);
        }
    }

    return buf;
}

void loadHead (LTA::Lta* ta,vector<Op>& head, map<string,int>& index)
{
    LTA::Location* cur = ta->getFirst ();
    Op op;
    op.init = cur->getDBM ();
    op.guard = cur ->getEdge ()->getDBM ();
    op.reset = convert (cur->getEdge ()->getReset (),index);
    head.push_back (op);
    do
    {
        cur = cur->getEdge ()->getNextLocation ();
        op.init = cur->getDBM ();
        op.guard = cur ->getEdge ()->getDBM ();
        op.reset = convert (cur->getEdge ()->getReset (),index);
        head.push_back (op);

    }
    while (!(cur->isLoopLoc ()));
}


void loadOps (LTA::Lta* ta,vector<Op>& head, vector<Op>& cycle,map<string,int>& index)
{
    LTA::Location* cur = ta->getFirst ();
    Op op;
    op.init = cur->getDBM ();
    op.guard = cur ->getEdge ()->getDBM ();
    op.reset = convert (cur->getEdge ()->getReset (),index);
    head.push_back (op);
    do
    {
        cur = cur->getEdge ()->getNextLocation ();
        op.init = cur->getDBM ();
        op.guard = cur ->getEdge ()->getDBM ();
        op.reset = convert (cur->getEdge ()->getReset (),index);
        head.push_back (op);

    }
    while (!(cur->isLoopLoc ()));
    cycle.push_back(op);
    do
    {


        cur = cur->getEdge ()->getNextLocation ();
        op.init = cur->getDBM ();
        op.guard = cur ->getEdge ()->getDBM ();
        op.reset = convert (cur->getEdge ()->getReset (),index);
        cycle.push_back (op);
    }
    while (!(cur->isLoopLoc ()));
}

int checkContainment (vector<VariableAssignment>& assigns, DBM& dbm)
{
    for (int i=0;i<assigns.size ();i++)
    {
      if (dbm.contained (assigns[i]))
            return i;
    }

    return -1;
}


void findHeadDelays(LinSequence*head, vector<mpq_class>& headD)
{
    Node* cur = head -> getFirst ();
    while (cur!=0)
    {
        headD.push_back (cur->getLeave ()[1] - ((cur==head->getFirst ()) ? 0 : cur->getEnter ()[1]));
        if (cur==head->getLast ())
            cur = 0;
        else
        {
            cur = cur->getNext ();
        }
    }
}

void findDelays (LinSequence*head, LinSequence* cycle, vector<mpq_class>& headD, vector<mpq_class>& cycleD) {
  Node* cur = head -> getFirst ();
  while (cur!=0) {
    headD.push_back (cur->getLeave ()[1] - ((cur==head->getFirst ()) ? 0 : cur->getEnter ()[1]));
    if (cur==head->getLast ())
      cur = 0;
    else {
      cur = cur->getNext ();
    }
  }
  cout.flush ();
  cur = cycle->getFirst ()->getNext();
  while (cur != 0) {
    cycleD.push_back (cur->getLeave()[1] - cur->getEnter ()[1]);
    if (cur == cycle->getLast ())
      cur = 0;
    else {
      cur = cur ->getNext ();
    }
  }

}


Result* solveLiveSimple (LTA::Lta* ta, Options* opt, istream& in,  ostream& out)
{
    vector<Op> headOps;

    loadHead(ta,headOps,opt->clockIndex);
    LinSequence* head = new LinSequence (headOps);
    head -> calcZones ();
    pair<VariableAssignment*, vector<mpq_class>* > cycleresult = solveLiveCycleSimple (ta, opt, in, out);
    head ->findAssignments (*(cycleresult.first));
    vector<mpq_class> headDelays;
    findHeadDelays (head,headDelays);
    return new LiveResult (headDelays ,*(cycleresult.second));
}


Result* solveLiveDummy (LTA::Lta* ta, Options* opt, istream& in,  ostream& out)
{
    vector<Op> headOps;
    loadHead(ta,headOps,opt->clockIndex);
    LinSequence* head = new LinSequence (headOps);
    head -> calcZones ();
    //pair<VariableAssignment*, vector<mpq_class>* > cycleresult = solveLiveCycleSimple (ta, opt, in, out);
    DBM last = head->getLast()->getUpped();
    VariableAssignment backtrackpoint = last.findPoint();
    vector<mpq_class> dummyLoopDelays;
    dummyLoopDelays.push_back(mpq_class(0));
    head ->findAssignments (backtrackpoint);
    vector<mpq_class> headDelays;
    findHeadDelays (head,headDelays);
    return new LiveResult(headDelays, dummyLoopDelays);
}


Result* solveLiveZone (LTA::Lta* ta, Options* opt, istream& in,  ostream& out) {
  vector<Op> cycleOps;
  vector<Op> headOps;

  vector<VariableAssignment> assigns;
  vector<LinSequence*> sequences;


  loadOps(ta,headOps,cycleOps,opt->clockIndex);
  LinSequence* head = new LinSequence (headOps);

  DBM cur = cycleOps[0].init;
  int i = -1;
  VariableAssignment v = cur.findPointMax ();

  assigns.push_back(v);
  while (i==-1) {

    LinSequence* seq = new LinSequence (v,cycleOps);
    sequences.push_back (seq);
    DBM d = seq->getLast()->getUpped ();

    i= checkContainment (assigns,d);
    if (i!= -1) {
      break;
    }
    else {
      v = d.findPointMax ();
      assigns.push_back (v);

    }
  }

  //Very well an assignment has been found...Let us create the full trace

  for (int j = i; j <assigns.size ();j++) {
    //cout << "Sequence: " << i << endl;
    sequences[i]->findAssignments(assigns[(j<assigns.size ()-1) ? j : i]);
  }
  head -> calcZones ();
  head ->findAssignments (assigns[i]);
  for (int j = i+1; j< assigns.size ();j++) {
    sequences[i] ->splice (sequences[j]);
  }

  vector<mpq_class> headD;
  vector<mpq_class> cycleD;

  findDelays (head,sequences[i],headD,cycleD);
  for (int j=0; j<assigns.size(); j++) {
    delete sequences[j];
  }

  delete head;


    return new LiveResult (headD,cycleD);
}

Result* solveLive (LTA::Lta* ta, Options* opt, istream& in,  ostream& out)
{    //return solveLiveZone (ta,opt,in,out);
    //else
    if (opt->solveOptions =="dummy")
    {
        return solveLiveDummy(ta,opt,in,out);
    }
    return solveLiveSimple(ta,opt,in,out);
}

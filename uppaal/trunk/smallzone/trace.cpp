#include <iostream>
#include <vector>
#include <sstream>
#include <gmpxx.h>

#include "smallzone/trace.hpp"
#include "solver/interface.hpp"
#include "solver/interfaceStat.hpp"
#include "main/Options.hpp"

#include "realDBM/dbm.hpp"
#include "realDBM/bound.hpp"
#include "symbTrace/lta.hpp"

using std::vector;
using std::istringstream;
using std::cout;
using std::endl;

int i = 0;

namespace smallzone
{
    Transition::~Transition ()
    {

        if (to!=0)
            delete to;
    }

    Transition::Transition (DBM g, vector<int> r,Node* f, Node* t) :  guard(g), reset (r)
    {
        from = f;
        to = t;

    }

    DBM& Transition::getGuard ()
    {
        return guard;
    }

    Node* Transition::getTo ()
    {
        return to;
    }

    Node* Transition::getFrom ()
    {
        return from;
    }



    vector<int>& Transition::getReset ()
    {
        return reset;
    }

    //Nodes implementation

    void Node::setEntry ( VariableAssignment v)
    {
        valEntry = v;
    }
    VariableAssignment Node::getEntryV ()
    {
        return valEntry;
    }
    void Node::setDelay ( VariableAssignment v)
    {
        valDelay = v;
    }
    VariableAssignment Node::getDelayV ()
    {
        return valDelay;
    }


    Node::Node (DBM e, DBM d) : entry (e), delay(d)
    {
        trans = 0;
    }

    Node::~Node ()
    {


        if (trans != 0) delete trans;
    }

    DBM& Node::getEntry ()
    {
        return entry;
    }

    DBM& Node::getDelay ()
    {
        return delay;
    }

    void Node::setBackward (Transition* t)
    {
        backTrans = t;
    }


    void Node::setForward (Transition* t)
    {
        trans = t;
    }

    Transition* Node::getBackward ()
    {
        return backTrans;
    }

    Transition* Node::getForward ()
    {
        return trans;
    }


    //Trace
    Options* opti;
    void Trace::add (DBM& inv, vector<int> resets, DBM& guard)
    {
        DBM lastDZone = last->getDelay ();
        DBM entryZone = lastDZone.intersect (guard).reset(resets);
        DBM delayZone = entryZone.up ().intersect (inv);

        //entryZone.output (cout,opti->clockIndex);

        Node* n = new Node (entryZone,delayZone);
        Transition* trans = new Transition (guard,resets,last,n);
        last->setForward (trans);
        n->setBackward (trans);
        last = n;
        states++;
    }

    vector<int> convert (string& s, map<string,int> index)
    {

        vector<int> buf;
        istringstream str (s);
        string buffer;
        while (std::getline(str, buffer, ','))
        {
            //cout << "Converting: " << "," << buffer << "," <<endl;
            if (buffer!= "")
            {

                buf.push_back (index[buffer]);
            }
        }

        return buf;
    }

    Trace::Trace (LTA::Lta* ta, Options* opt)
    {
        states = 0;
        opti = opt;
        int numClocks = ta->getClockNames ()->size ();
        DBM zeroD (numClocks);
        Bound zeroBound (mpq_class(0),false);
        int it;
        int it2;
        bool setRef = false;
        states ++;

        for (it = 1;it!=numClocks;it++)
        {
            zeroD.setConstraint (0,it,zeroBound);
            zeroD.setConstraint (it,0,zeroBound);
            for (it2 = 1;it2!=numClocks;it2++)
            {
                zeroD.setConstraint (it,it2,zeroBound);
                zeroD.setConstraint (it2,it,zeroBound);
            }
        }
        zeroD.close (true);
        // zeroD.output (cout,opt->clockIndex);
        //cout << endl << endl;
        LTA::LtaIterator* ltait = ta->getIterator ();
        DBM inv = ltait->getLocation () ->getDBM ();
        DBM delayed = zeroD.up ().intersect (inv);
        first =  new Node (zeroD,delayed);
        last = first;
        do
        {
            if (ltait->getEdge ()!= 0 && ltait->getEdge()->getNextLocation ()!=0)
            {
                string s = ltait->getEdge ()->getReset ();
                vector<int> reset =  convert (s,opt->clockIndex);

                DBM guard = ltait->getEdge () ->getDBM ();
                inv = ltait->getEdge()->getNextLocation ()->getDBM ();
                add (inv,reset,guard);
            }

        }
        while (ltait->move ());
        delete ltait;

    }

    string Trace::getRefClock ()
    {
        return refClock;
    }
}



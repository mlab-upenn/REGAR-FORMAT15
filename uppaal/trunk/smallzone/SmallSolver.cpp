#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <time.h>
#include <ctime>
#include "solver/interface.hpp"
#include "solver/interfaceStat.hpp"

#include "symbTrace/lta.hpp"
#include "symbTrace/Location.hpp"
#include "symbTrace/Edge.hpp"
#include "rationalDBM/dbm.hpp"
#include "finiteResult/dumbResult.hpp"

using std::vector;
using std::cout;
using std::endl;
using std::ostream;

using namespace LTA;
using namespace newdbm;

struct EntryAndLeave
{
    VariableAssignment enter;
    VariableAssignment leave;
};

ostream & operator<< (ostream & nycout, VariableAssignment& varass)
{
    for(int i=0; i < varass.size(); i++)
    {
        nycout << varass[i] << " ";
    }
    nycout << endl;
    return nycout;
}



VariableAssignment doReset (VariableAssignment v, const vector<int>& reset )
{
    for (int i= 0;i<reset.size ();i++)
    {
      //cout << "Resetting: " << reset[i] <<  endl;
        v[reset[i]] = 0;
    }
    return v;
}

mpq_class FindDelayDifference(EntryAndLeave info)
{
    mpq_class res = info.leave[1] - info.enter[1];
    for(int i=2; i < info.enter.size(); i++)
    {
        if(info.leave[i] - info.enter[i] != res)
        cout << "Error in finding delays!" << endl;
    }
    cout << "DelayDifference found " << endl;
    return res;
}


Result* solveSmallForw (LTA::Lta* ta, Options* opt, istream& in,  ostream& out)
{
    vector<EntryAndLeave> entries;
    Location* cur = ta->getFirst ();
    while (cur!= 0)
    {
        EntryAndLeave buffer;
        if (cur==ta->getFirst())
        {
            DBM enterZone = DBM::initZone (cur->getDBM ().numOfVars ());
            buffer.enter = enterZone.findPoint ();
        }

        else
        {
            //Perform rese of last states leave

            buffer.enter = doReset(entries.back ().leave,cur->getBack ()->getResetVector());
        }

        //Leave
        DBM delayZone;
        if (cur->getEdge ()!=0)
            delayZone = DBM(buffer.enter).up ().intersect (cur->getEdge ()->getDBM ()).intersect (cur->getDBM());
        else
            delayZone = DBM(buffer.enter).up ().intersect (cur->getDBM());
        buffer.leave = delayZone.findPoint ();

        entries.push_back (buffer);
        if (cur->getEdge ()!=0)
            cur = cur->getEdge ()->getNextLocation ();
        else
        {
            cur = 0;
        }
    }

    vector<mpq_class> delays;

    for (int i= 0;i< entries.size ();i++)
    {
        delays .push_back (FindDelayDifference(entries[i]));
    }

    return new DumbResult (delays);
}




Result* solveSmallBack (LTA::Lta* ta, Options* opt, istream& in,  ostream& out)
{

    //First make the links backwards on the ta
    //ta->makeBackLinks ();
    //Now we need to find the last state
  mpq_class myEps = (mpq_class(1)/opt->epsilon)/ta->getNumberOfLocations ();
  Location* cur = ta->getFirst ();
  while (cur->getEdge () != 0)
    {
      
      cur = cur->getEdge ()->getNextLocation ();
    }
  

    //Cur now point to the last location....
    //Iterate backwards through the states and find valuations

  vector<EntryAndLeave> entries;
    EntryAndLeave buffer;
    bool laststate = true;
    int iter=0;
    while (cur != 0)
    {

        iter++;
        if (laststate)
        {
	     
 	    buffer.leave = cur->getDBM().findPointMinClock ( opt->clockIndex[opt->globalC],(mpq_class(1)/opt->epsilon));
	
	    //Find entry point
	    laststate = false;
        }
        else
        {

            DBM actionPredecessor =
                DBM(entries.back().enter).
                free(cur->getEdge ()->getResetVector()).
                intersect (cur->getEdge ()->getDBM ()).
                intersect (cur->getDBM ());
            buffer.leave = actionPredecessor.findPointMin (myEps);

            //cout.flush ();
        }
        //Construct time predecessor
        DBM enterZone;
        if (cur==ta->getFirst ())
        {

            enterZone = DBM::initZone (cur->getDBM ().numOfVars ());

        }
        else
        {
            enterZone = cur->getBack ()->getPrev ()->getDBM ().
                        intersect (cur->getBack()->getDBM ()).
                        reset (cur->getBack()->getResetVector ());
        }

        DBM timePredecessor = DBM (buffer.leave).down ().intersect(enterZone);
        buffer.enter = timePredecessor.findPointMax (myEps);
	
        if (cur!= ta->getFirst ())
            cur = cur->getBack ()->getPrev ();
        else
            cur = 0;
        entries.push_back (buffer);

    }
    vector<mpq_class> delays;

    for (int i= entries.size ()-1;i>=0;i--)
    {
        delays .push_back (entries[i].leave[1] - entries[i].enter[1]);;
    }

    return new DumbResult (delays);
}
Result* solveSmall  (LTA::Lta* ta, Options* opt, istream& in,  ostream& out)
{
    if (opt->solveOptions == "back")
    {
        if (!opt->quiet)
            out << "Doing search backward" << endl;
        return solveSmallBack (ta,opt,in,out);
    }

    else if (opt->solveOptions== "forw")
    {
        if (!opt->quiet)
            out << "Doing search forward"<< endl;
        return solveSmallForw (ta,opt,in,out);
    }

    else
    {
        if (!opt->quiet)
            out << "Doing  standard seach:  Backward" << endl;
        return solveSmallBack (ta,opt,in,out);
    }
}

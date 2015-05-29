#include <sstream>
#include <set>
#include <vector>
#include <map>
#include <time.h>
#include <ctime>
#include "solver/interface.hpp"
#include "solver/interfaceStat.hpp"
#include <climits>
#include "symbTrace/lta.hpp"
#include "symbTrace/Location.hpp"
#include "rationalDBM/dbm.hpp"
#include "rationalDBM/bound.hpp"
#include "liveness/liveResult.hpp"
#include "liveness/liveTrace.hpp"
#include "utility"

using std::pair;
using std::istream;
using std::ostream;
using std::stringstream;
using std::endl;
using std::vector;
using std::set;
using std::map;
using std::cout;
using namespace LTA;
using namespace liveness;


// Takes as input a valuation and a DBM with only nonstrict bounds.
// The valuation must be in the DBM.
// The function returns the maximal delay d that can be added to the valuation
// such that the valuation + d is in the DBM.
mpq_class FindMaxDelay(VariableAssignment &point, DBM &dbm)
{
    mpq_class maxdelay = mpq_class(INT_MAX);
    for (int i=1; i < dbm.numOfVars(); i++)
    {
        Bound bound = dbm.getBound(i,0);
        mpq_class upperbound = bound.getBound();
        if (upperbound - point[i] < maxdelay)
            maxdelay = upperbound - point[i];
    }
    return maxdelay;
}

/* Takes as input a valuation and adds delays to the valuation.
 */
void DelayPoint(VariableAssignment &point, mpq_class& delay)
{
    int temp;
    for (int i=1; i < point.size(); i++)
    {
        point[i] = point[i] + delay;
    }
}


// Returns the index of the valuation if it is contained in the vector of points.
// Returns -1 otherwise
int PointVisited(VariableAssignment &point, vector<VariableAssignment> &visitedPoints)
{
    bool equal;
    for (int i=0; i < visitedPoints.size(); i++) // for each point in visitedPoints
    {
        equal = true;
        for (int j=1; j < visitedPoints[i].size(); j++) // for each value in it
        {
            if ( visitedPoints[i][j] != point[j] )  // if point a different value as the point in visitedPoints
            {
                equal = false;
                break;   // no reason to check the other variables
            }
        }
        if (equal)
            return i;
    }
    return -1;
}


/* Sets point to the variableassignment where all clocks
 * have the maximal allowed value.
 * Requires that all bounds in the DBM are nonstrict and
 * that they are not infinity.
 */
void maxPointInZone(DBM &dbm, VariableAssignment& point)
{
    point[0] = mpq_class(0);
    for (int i=1; i < dbm.numOfVars(); i++)
    {
        Bound bound = dbm.getBound(i,0);
        point[i] = bound.getBound();
    }
}

// Takes several vectors containing integers and appends these to eachother
vector<mpq_class>* CombineTraceParts(int fromIndex, vector<vector<mpq_class> > &delayFor1Iteration)
{
    vector<mpq_class>* result = new vector<mpq_class>();
    for (;fromIndex < delayFor1Iteration.size(); fromIndex++)
    {
        for (int i=0; i < delayFor1Iteration[fromIndex].size(); i++)
        {
            result->push_back(delayFor1Iteration[fromIndex][i]);
        }
    }
    return result;
}

// Given a symbolic cycle, returns a point that can reach itself by performing maximal delays (which are returned as the second part of the result)
pair<VariableAssignment*, vector<mpq_class>* > solveLiveCycleSimple(Lta* ta, Options* opt, istream& in,  ostream& out)
{
    map<string,int> cindex = opt->clockIndex;

    Location* loc = ta->getFirst();
    while(!loc->isLoopLoc())
    {
        loc = loc->getEdge()->getNextLocation();
    }
    DBM dbm = loc->getDBM();
    VariableAssignment point = VariableAssignment(dbm.numOfVars());
    maxPointInZone(dbm, point);
    vector<int> resets;
    mpq_class delay;
    vector<mpq_class> delays;
    vector<vector<mpq_class> > delaysFor1Iteration;

    vector<VariableAssignment> visitedPoints;
    visitedPoints.push_back(point);

    int whilecounter=0;

    while (true) // no duplicate point
    {
        delays = vector<mpq_class>();
        do // go through one iteration and find the delays
        {
            resets = loc->getEdge()->getResetVector();
            for (int i=0; i < resets.size(); i ++)
            {
                point[resets[i]]= mpq_class(0);
            }
            loc = loc->getEdge()->getNextLocation();
            dbm = loc->getDBM();
            delay = FindMaxDelay(point, dbm);
            DelayPoint(point,delay);
            delays.push_back(delay);
        }
        while (!loc->isLoopLoc());
        delaysFor1Iteration.push_back(delays);

        whilecounter++;
        int visited = PointVisited(point, visitedPoints);
        if (visited != -1)
        {
            pair<VariableAssignment*, vector<mpq_class>* > result;
            result.first = new VariableAssignment(point);
            result.second = CombineTraceParts(visited, delaysFor1Iteration);
            if(opt->time)
            {
                cout << "While loop performed " << whilecounter << " times." << endl;
                cout << "Result has k-value: " << delaysFor1Iteration.size() - visited << " times." << endl;
            }
            return result;
        }
        else
            visitedPoints.push_back(point);
    }
}

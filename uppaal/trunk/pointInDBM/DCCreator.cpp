/*
		CTU - Concrete Traces for Uppaal.
    Copyright (C) 2010  CTU team

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "pointInDBM/DCCreator.hpp"
#include "dbm/fed.h"
#include "symbTrace/DCConstraint.hpp"
#include "symbTrace/lta.hpp"
#include <stdlib.h>
#include <map>
#include <vector>
#include <list>
#include <time.h>
#include "dbm/dbm.h"
#include "header.hpp"
// To make the code more readable
#define DBM(I,J) rawdbm[(I)*dim+(J)]

using namespace DCCREATOR;
using namespace LTA;
using namespace dbm;
using namespace std;

extern bool doTime;
extern timeKeeper keep;
/* Returns whether an entry x in the DBM
 * is smaller than an entry y.
 */
bool isLess(raw_t& x,raw_t&y) {
  bool strictnessX = dbm_rawIsStrict (x);
  int boundX = dbm_raw2bound (x);
  int boundY = dbm_raw2bound (y);

  if (boundX< boundY)
    return true;
  else if (boundX == boundY && strictnessX)
    return true;
  else
    return false;
}

/* Constructor:
 * When we create the DCCreator, we need an LTA and the
 * zero clock representation.
 */
DCCreator::DCCreator(Lta* inputlta, string &zeroclock)
{
    lta = inputlta;
    zeroClockRepresentation = zeroclock;
    //Allocate space for last reset at data
    lastresetdata = (int*) calloc (lta->getNumberOfLocations() * lta->getNumberOfClocks(),sizeof(int));
    if (lastresetdata==0)
    {
        cerr << "No memory obtained" << endl;
        cerr.flush ();
    }

    for (unsigned int i = 0; i < lta->getNumberOfClocks(); i++)
    {
        clocknumbermap[(*(lta->getClockNames()))[i]] = i;
    }
    CreateLastResetAtTable();
}

DCCreator::~DCCreator ()
{
    delete lastresetdata;
}

/* Corresponds to the algorithm for creating entry
 * time constraints in our report.
 */
dbm_t& DCCreator::CreateEntryTimeConstraints()
{
  clock_t start;
  if (doTime)
    start = clock ();
  //Create and initiliaze the dbm
  int dim = lta->getNumberOfLocations();
  entryTimeDBM = dbm_t(dim);
  raw_t* rawdbm= entryTimeDBM.getDBM ();
  dbm_init (rawdbm,dim);

  LtaIterator *ltaIterator = lta->getIterator();

  cindex_t n = lta->getNumberOfLocations();
  const list<dc_t*> *invariant, *guard;
  const Location* loc;

  for (cindex_t i=0; i<n; i++)
    {
      loc = ltaIterator->getLocation();
      invariant = loc->getInvariant();
      CreateConstraint(rawdbm, dim, invariant, i, false);
      ltaIterator->move();
    }

  ltaIterator = lta->getIterator();
  for (cindex_t i=0; i<n-1; i++)
    {
      loc = ltaIterator->getLocation();
      DBM(i, i+1) = dbm_bound2raw(0, dbm_WEAK);

      guard = loc->getEdge()->getGuard();
      CreateConstraint(rawdbm, dim, guard, i, true);

      invariant = loc->getInvariant();
      CreateConstraint(rawdbm, dim, invariant, i, true);

      ltaIterator->move();
    }

  if (doTime) {
    keep.createConstraint = (double)(clock()-start)/CLOCKS_PER_SEC;
    start = clock ();
  }

    dbm_close (rawdbm,dim);
    if (doTime) {
       keep.closeDBM = (double)(clock()-start)/CLOCKS_PER_SEC;
    }
    delete ltaIterator;
    return entryTimeDBM;
}

/* Function that takes a guard or invariant and converts it to
 * an entry time constraint and inserts this into a.
 */
void DCCreator::CreateConstraint(raw_t *rawdbm, int dim, const list<dc_t*> *clockConstraints, int location, bool compareToNext)
{
    string i,j;
    list<dc_t*>::const_iterator iterator;
    int aheadInTime;

    if (compareToNext)
        aheadInTime = 1;
    else
        aheadInTime = 0;


    for (iterator=clockConstraints->begin(); iterator != clockConstraints->end(); iterator++ )
    {
        i = (*iterator)->getX();
        j = (*iterator)->getY();
        bool strict;

        if((*iterator)->getOp() == lt)
            strict = true;
        else // if(iterator->getOp() == leq)
            strict = false;

        if (i != zeroClockRepresentation && j != zeroClockRepresentation)
        {
            constrainEntry(rawdbm, dim, LastResetAt(location, j), LastResetAt(location, i), (*iterator)->getBound(), strict);
        }
        else if (i==zeroClockRepresentation)
        {
            constrainEntry(rawdbm, dim, LastResetAt(location, j), location + aheadInTime, (*iterator)->getBound(), strict);
        }
        else  // if (y==zeroClockRepresentation)
        {
            constrainEntry(rawdbm, dim, location + aheadInTime, LastResetAt(location, i), (*iterator)->getBound(), strict);
        }
    }
}

/* Function takes as input a dbm and a new constraint.
 * It checks whether the new constraint is tighter
 * than the constraint in the dbm and replaces it
 * this is the case.
*/
void DCCreator::constrainEntry(raw_t *rawdbm, int dim, int i, int j, int bound, bool isStrict)
{
    raw_t newval;
    if(isStrict)
        newval = dbm_bound2raw(bound, dbm_STRICT);
    else
        newval = dbm_bound2raw(bound, dbm_WEAK);

    if(isLess(newval,DBM(i,j)))
        DBM(i,j) = newval;
}

/* Implementation of the last-reset-at function
 * used in our report.
 */
int DCCreator::LastResetAt(int locId, string& clock)
{
    int clockNr = clocknumbermap[clock];
    return lastresetdata[locId*(lta->getNumberOfClocks ())+clockNr];
}


/* Initialization of the data used in the
 * last-reset-at function used in our report.
 * Requires the clocks reset to be separated by commas,
 * as it might otherwise detect clock xy
 * is reset when clock xyz is reset!
 */

void DCCreator::CreateLastResetAtTable()
{
    LtaIterator *ltaIterator = lta->getIterator();
    const Edge* edge;
    unsigned int location, clocknr;
    int pos;
    vector<string> clocknames;

    for (clocknr = 0; clocknr < lta->getNumberOfClocks(); clocknr++)
    {
        lastresetdata[clocknr] = 0;
    }

    unsigned int locs =  lta->getNumberOfLocations();
    unsigned int clocks = lta->getNumberOfClocks();
    for (location = 1; location < lta->getNumberOfLocations(); location++)
    {
        edge = ltaIterator->getEdge();
        for (clocknr = 0; clocknr < lta->getNumberOfClocks(); clocknr++)
        {
            pos = edge->getReset().find("," + (*(lta->getClockNames()))[clocknr] + ",");
            if (pos == -1)
            {
                lastresetdata[(location*clocks)+clocknr] = lastresetdata[((location-1)*clocks)+clocknr];
            }
            else
            {
                lastresetdata[location*clocks+clocknr] = location;
            }
        }
        ltaIterator->move();
    }
}

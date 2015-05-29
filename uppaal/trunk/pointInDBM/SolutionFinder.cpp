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

#include "dbm/fed.h"
#include "dbm/dbm.h"
#include "dbm/constraints.h"
#include <pointInDBM/DCCreator.hpp>
#include "solver/interface.hpp"
#include "rationalDBM/dbm.hpp"
#include "finiteResult/dumbResult.hpp"
#include "header.hpp"
#include <gmpxx.h>  // GNU bignum lib


using namespace dbm;
using namespace DCCREATOR;
using namespace std;

extern bool scaleInteger ;
extern bool doTime;
extern bool integer;

clock_t start;
clock_t end;

timeKeeper keep;

#define DBM(I,J) raw[(I)*dim+(J)]

void ConvertEntryTimesToDelays(VariableAssignment& entryTimes, VariableAssignment& delays)
{
    for (int i=0; i < entryTimes.size()-1; i++)
    {
        delays[i] = entryTimes[i+1] - entryTimes[i];
    }
}

mpq_class findValue (bool lStrict, mpq_class& lower, bool ustrict, mpq_class& upper, mpq_class& increase)
{
    if (!lStrict)
    {
        return lower;
    }
    else  // We know for sure that there exists a solution, so the range is surely not empty
    {
        return lower + increase;
    }
}

// Finds a fastest trace by setting the last variable first
Result* findFastestPoint (dbm_t& dbm, mpz_class& epsilon)
{

    if (doTime)
        start = clock ();
    cindex_t dim = dbm.pdim ();
    VariableAssignment entryTimes = VariableAssignment(dim);
    bool restricted[dim];
    raw_t* raw = dbm.getDBM ();

    //Initialize all variables to be free
    for (cindex_t i=0;i<dim;i++)
    {
        restricted[i] = false;
    }

    mpq_class upper, lower;
    mpq_class increase = (mpq_class(1) / epsilon)/dim;

    //First of all we restrict that entering the first state must be = 0;
    entryTimes[0] = mpq_class(0);
    //and hence it is restricted
    restricted [0] = true;

    bool lstrict;
    bool ustrict;
    // Go through all variables that are free
    for (int i=1;i<dim;i++)
    {
        if (!restricted[i])
        {
            // Find the lower and upper bound from the dbm
            ustrict= dbm_rawIsStrict (DBM(i,0));
            lstrict= dbm_rawIsStrict (DBM(0,i));
            upper = mpq_class(mpq_class(dbm_raw2bound (DBM(i,0))));
            lower = mpq_class(mpq_class(-dbm_raw2bound (DBM(0,i))));

            // Derive tighter bounds if possible
            for (int j=1;j<dim;j++)
            {
                if (restricted[j] && i != j)
                {
                    bool boundStrict = dbm_rawIsStrict (DBM(i,j));
                    mpq_class bound = mpq_class (dbm_raw2bound(DBM(i,j))) + entryTimes[j];
                    if ((bound < upper || bound == upper && boundStrict))
                    {
                        ustrict = boundStrict;
                        upper = bound;
                    }
                    boundStrict = dbm_rawIsStrict (DBM(j,i));
                    bound = mpq_class(-dbm_raw2bound(DBM(j,i))) + entryTimes[j];

                    if (bound > lower  || (bound == lower && boundStrict))
                    {
                        lstrict = boundStrict;
                        lower = bound;
                    }
                }
            }
            // We now have the tighest bounds. Choose something between these values
            entryTimes[i] =  findValue (lstrict,lower,ustrict,upper, increase);
            restricted[i] = true;
        }
    }
    if (doTime)
    {
        end = clock() ;
        keep.findPoint = (double)(end-start)/CLOCKS_PER_SEC;
    }
    VariableAssignment delays = VariableAssignment(dim-1);
    ConvertEntryTimesToDelays(entryTimes, delays);

    Result* res =  new DumbResult(delays);
    return res;
}

// Finds an integer delay if possible, and chooses a short delay otherwise
mpq_class findIntValue (bool lstrict, mpq_class& lower, bool ustrict, mpq_class& upper, mpq_class& increase, mpq_class& prev)
{
    // Heuristics - try to make an integer delay
    mpq_class diff = lower - prev;
    mpq_class intentry;
    mpz_t times;
    mpz_init (times);
    cout.flush();
    mpz_cdiv_q (times, diff.get_num_mpz_t(), diff.get_den_mpz_t());
    mpz_class nrOfTimes = mpz_class(times);
    intentry= nrOfTimes + lower;

    if ((intentry == lower && lstrict))
    {
        intentry = intentry+mpq_class(1);
    }
    if ((intentry < upper) || (intentry < upper && !ustrict))
    {
        return intentry;
    }
    else  // we cannot choose an integer
    {
        if (!lstrict)
        {
            return lower;
        }
        mpq_class increasedlower = lower + increase;
        if (increasedlower < upper || (increasedlower == upper && !ustrict))// We know for sure that there exists a solution, so the range is surely not empty
        {
            return increasedlower;
        }
        else
        {
            return (lower + upper) / 2;
        }
    }
}

// Finds a fastest trace with some integer delays
Result* findFastestIntDelayPoint (dbm_t& dbm, mpz_class& epsilon)
{

    if (doTime)
        start = clock ();
    cindex_t dim = dbm.pdim ();
    VariableAssignment entryTimes = VariableAssignment(dim);
    bool restricted[dim];
    raw_t* raw = dbm.getDBM ();

    //Initialize all variables to be free
    for (cindex_t i=0;i<dim;i++)
    {
        restricted[i] = false;
    }

    mpq_class upper, lower;
    mpq_class increase = mpq_class(1) / epsilon;

    //First of all we restrict that entering the first state must be = 0;
    entryTimes[0] = mpq_class(0);
    //and hence it is restricted
    restricted [0] = true;
    bool lstrict;
    bool ustrict;
    // Go through all variables that are free
    for (int i=1;i<dim;i++)
    {
        if (!restricted[i])
        {
            // Find the lower and upper bound from the dbm
            ustrict= dbm_rawIsStrict (DBM(i,0));
            lstrict= dbm_rawIsStrict (DBM(0,i));
            upper = mpq_class(mpq_class(dbm_raw2bound (DBM(i,0))));
            lower = mpq_class(mpq_class(-dbm_raw2bound (DBM(0,i))));

            // Derive tighter bounds if possible
            for (int j=1;j<dim;j++)
            {
                if (restricted[j] && i != j)
                {
                    bool boundStrict = dbm_rawIsStrict (DBM(i,j));
                    mpq_class bound = mpq_class (dbm_raw2bound(DBM(i,j))) + entryTimes[j];
                    if ((bound < upper || bound == upper && boundStrict))
                    {
                        ustrict = boundStrict;
                        upper = bound;
                    }
                    boundStrict = dbm_rawIsStrict (DBM(j,i));
                    bound = mpq_class(-dbm_raw2bound(DBM(j,i))) + entryTimes[j];

                    if (bound > lower  || (bound == lower && boundStrict))
                    {
                        lstrict = boundStrict;
                        lower = bound;
                    }
                }
            }
            // We now have the tighest bounds. Choose something between these values
            entryTimes[i] =  findIntValue (lstrict,lower,ustrict,upper, increase, entryTimes[i-1]);
            restricted[i] = true;
        }
    }
    if (doTime)
    {
        end = clock() ;
        keep.findPoint = (double)(end-start)/CLOCKS_PER_SEC;
    }
    VariableAssignment delays = VariableAssignment(dim-1);
    ConvertEntryTimesToDelays(entryTimes, delays);

    Result* res =  new DumbResult(delays);
    return res;
}


Result* FindSolution(dbm_t& entryTimeDBM, mpz_class& epsilon)
{
    // We dont have to close the dbm, as the closed form is maintained while it is build (see fed.h)
    if (integer)
    {
        return findFastestIntDelayPoint(entryTimeDBM,epsilon);
    }
    else
    {
        return findFastestPoint(entryTimeDBM,epsilon);
    }
}


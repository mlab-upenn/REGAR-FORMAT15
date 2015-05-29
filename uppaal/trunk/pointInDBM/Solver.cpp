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

#include "solver/interface.hpp"
#include "pointInDBM/DCCreator.hpp"
#include "dbm/fed.h"
#include "dbm/fed.h"
#include "dbm/dbm.h"
#include "dbm/constraints.h"
#include "main/Options.hpp"
#include "solver/interfaceStat.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <gmpxx.h>
#include <time.h>
#include "header.hpp"

using std::string;
using std::istringstream;
using std::istream;
using std::cout;
using std::endl;
using std::ostream;
using std::vector;
using namespace dbm;
using namespace DCCREATOR;
bool doTime = false;
extern timeKeeper keep;

Result* FindSolution(dbm_t& entryTimeDBM, mpz_class& epsilon);

mpz_class epsilon = mpz_class (10);
bool scaleInteger = false;
bool integer = false;


#define DBM(I,J) dbm[(I)*dim+(J)]


Result* solvePoint (LTA::Lta* lta, Options* opt, istream& inputstream,  ostream& outputstream) {
  return solve(lta,opt,inputstream,outputstream);
}

extern "C" {

  Result* solve (LTA::Lta* lta, Options* opt, istream& inputstream,  ostream& outputstream) {
    //Options set up....
    //Create DCConstraints
    if (opt->solveOptions=="integer")
      integer = true;
    doTime = opt->time;
    string zeroRep (opt->zeroRep);
    clock_t start;
    DCCreator creator (lta,zeroRep);
    dbm_t entryTimeDBM = creator.CreateEntryTimeConstraints();
    Result* res= FindSolution (entryTimeDBM,opt->epsilon);
    return res;

  }
}

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
#include <string>
#include <iostream>
#include <sstream>
using std::string;
using std::cout;
using std::endl;
using std::ostringstream;
using std::istringstream;

#include "symbTrace/Edge.hpp"
#include "symbTrace/Location.hpp"
#include "symbTrace/DCConstraint.hpp"

#include "rationalDBM/dbm.hpp"
#include "rationalDBM/bound.hpp"

using namespace newdbm;



void createDBM (list<dc_t*>*g, DBM& dbm,map<string,int> cindex) {
  list<dc_t*>::iterator it;
  dbm = DBM ( cindex.size ());
  for (it = g->begin (); it!=g->end ();it++) {
    DCOperator comp = (*it)->getOp ();
    int x = cindex[(*it)->getX ()];
    int y = cindex[(*it)->getY ()];
    int bound = (*it)->getBound ();
    if (comp == lt)
      dbm.constrain (x,y,Bound (mpq_class (bound),true));
    else if (comp == leq)
      dbm.constrain (x,y,Bound (mpq_class (bound),false));
    else {
      dbm.constrain (x,y,Bound (mpq_class (bound),false));
      dbm.constrain (y,x,Bound (mpq_class (-1*bound),false));
    }

  }

  dbm.close ();
}

 vector<int> convert (const string& s, map<string,int> index)
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

LTA::Edge::Edge (const string& r, list<dc_t*>*g  , Location* n,string i, map<string, int> cindex) : reset (r)
{
  nextLocation = n;
  constraints = g;
  id = i;
  createDBM (constraints,dbm, cindex);
  isLoop = false;
  resetVector = convert(r, cindex);
}

LTA::Edge::Edge (const string& r,  list<dc_t*>*g,string i, map<string, int> cindex) : reset (r) {
  constraints = g;
  id = i;
  createDBM (constraints,dbm, cindex);
  isLoop = false;
  resetVector = convert(r, cindex);
}

LTA::Edge::~Edge () {
  delete constraints;
  if (nextLocation != 0 && !isLoop) {
    delete nextLocation ;
  }
}


void LTA::Edge::setLoop () {
    isLoop = true;
  }

const DBM& LTA::Edge::getDBM () const  {
  return dbm;
}


void LTA::Edge::setNextLocation (Location* n) {

  nextLocation = n;
}

LTA::Location* LTA::Edge::getNextLocation () {
  return nextLocation;
}

const LTA::Location* LTA::Edge::getNextLocation ()  const {
  return nextLocation;
}

const string& LTA::Edge::getReset () const {
  return reset;
}


const list<dc_t*>* LTA::Edge::getGuard () const  {
  return constraints;
}

string LTA::Edge::getId () const  {
  return id;
}


string LTA::Edge::getGuardText () const {
  list<dc_t*>::const_iterator it;
  ostringstream str;
  for (it = constraints->begin ();it!=constraints->end();it++) {
    str << *(*it) << "&&";
  }


  return str.str ();
}

LTA::Location* LTA::Edge::getPrev () {
  return prev;
}

void LTA::Edge::setPrev(LTA::Location* l) {
  prev = l;
}

const vector<int>& LTA::Edge::getResetVector() const{
  return resetVector;
}

bool LTA::Edge::isLoopTrans () {
  return isLoop;
}

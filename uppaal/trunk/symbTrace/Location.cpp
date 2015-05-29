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
#include "symbTrace/Location.hpp"
#include "symbTrace/Edge.hpp"
#include "rationalDBM/dbm.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <list>
using std::list;
using std::string;
using std::cout;
using std::ostringstream;

using namespace newdbm;

LTA::Location::Location (const list<dc_t*>* in, Edge* e)  {
  inv = in;
  edge = e;
  isLoop = false;
}

LTA::Location::Location (const list<dc_t*>* in)  {
  edge = 0;
  inv = in;
  isLoop = false;
 
}

void LTA::Location::setLoop () {
  isLoop = true;
}

bool LTA::Location::isLoopLoc () {
  return isLoop;
}

LTA::Location::~Location () {
  if (edge!=0) 
    delete edge;
  delete inv;
}

LTA::Edge* LTA::Location::getEdge () {
  return edge;
}

const LTA::Edge* LTA::Location::getEdge () const {
  return edge;
}

const list<dc_t*>* LTA::Location::getInvariant () const {
  return inv;
}

void LTA::Location::setEdge (Edge* e) {
  edge = e;
}

string LTA::Location::getInvariantText () const {
  list<dc_t*>::const_iterator it;
  ostringstream str;
  for (it = inv->begin ();it!=inv->end();it++) {
    str << *(*it) << "&&";
  }


  return str.str ();
}

const DBM& LTA::Location::getDBM () const  {
 
  return dbm;
}


void LTA::Location::setDBM (DBM m) {

  dbm = m;
}

void LTA::Location::setBackLoop (Edge* e) {
  loopBack = e; 
}

void LTA::Location::setBack (Edge* e) {
  back = e;
}

LTA::Edge* LTA::Location::getBackLoop () {
  return loopBack;
}


LTA::Edge* LTA::Location::getBack () {
  return back;
}



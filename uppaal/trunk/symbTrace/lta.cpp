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
#include "symbTrace/lta.hpp"


#include <string>
#include <list>
#include <map>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

LTA::Lta::Lta (Location* n, vector<string>* c) {
  firstLocation = n;
  curLocation = n;
  numOfLocations =1;
  clocks = c;

}

unsigned int LTA::Lta::getNumberOfClocks () {
  return clocks->size ();
}

vector<string>* LTA::Lta::getClockNames () {
  return clocks;
}

LTA::Lta::~Lta () {
  if (firstLocation!=0)
    delete (firstLocation);
}

void LTA::Lta:: add (Edge* e, Location* n) {
  e->setNextLocation (n);
  e->setPrev (curLocation);
  if (e->isLoopTrans())
    n->setBackLoop (e);
  else 
    n->setBack (e);
  curLocation->setEdge (e);
  curLocation = n;
  numOfLocations++;


}

LTA::LtaIterator* LTA::Lta::getIterator () {
  return new LTA::LtaIterator (firstLocation);;
}

LTA::LtaIterator* LTA::Lta::getLoopIterator () {
  return new LTA::LtaIterator (loopStartLocation);;
}


unsigned int LTA::Lta::getNumberOfLocations () {
  return numOfLocations;
}


LTA::LtaIterator::LtaIterator (Location* n) {
  cur = n;
}

bool LTA::LtaIterator::hasSomething () {
  return cur!=0;
}

bool LTA::LtaIterator::move () {
  if (cur->getEdge ()==0 || cur->getEdge ()->getNextLocation ()==0)
    return false;
  else {
    cur  =cur->getEdge ()->getNextLocation ();
    return true;
  }
}
/*
void LTA::Lta::makeBackLinks () {

  bool inLoop = false;
  Location* cur = firstLocation;
  Edge* prevEdge = 0;
  while (cur->getEdge () != 0) {
    Edge* transition = cur->getEdge ();
    transition ->setPrev (cur);
    if(!cur->isLoopLoc() &&  !inLoop){
      cur->setBack (prevEdge);
     
      inLoop = true;
      }
    
    else {
      cur->setBackLoop (prevEdge);
      }
    prevEdge = transition;
    cur = cur->getEdge()->getNextLocation ();
	
  }
  
  cur->setBack (prevEdge);
}
*/
const LTA::Location* LTA::LtaIterator::getLocation () {
  return cur;
}

const LTA::Edge* LTA::LtaIterator::getEdge() {
  return cur->getEdge ();
}



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
#ifndef _LTA__
#define _LTA__

#include "include/symbTrace/Location.hpp"
#include "include/symbTrace/Edge.hpp"

#include <vector>

using std::vector;


namespace LTA {


  class LtaIterator {
  private:
    const Location* cur;
  public:
    LtaIterator (Location*);
    bool move();
    const Location* getLocation ();
    const Edge* getEdge ();
    bool hasSomething ();
  };

  class Lta {
  private:
    Location* firstLocation;
    Location* curLocation;
    Location* loopStartLocation;
    unsigned numOfLocations;
    vector<string>* clocks;
  public:
    Lta (Location*,vector<string>*);
    ~Lta ();
    void add (Edge*, Location*);
    void setLoopStartLocation(Location* loc) { loopStartLocation = loc; }
    unsigned int getNumberOfLocations ();
    unsigned int getNumberOfClocks ();
    vector<string>* getClockNames ();
    LtaIterator* getIterator ();
    LtaIterator* getLoopIterator();
    Location* getFirst () {return firstLocation;}
    void makeBackLinks ();
  };
}

#endif



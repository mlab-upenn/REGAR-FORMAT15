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
#ifndef _EDGE__
#define _EDGE__

#include <string>
#include <map>

#include <list>
#include "symbTrace/DCConstraint.hpp"


#include "rationalDBM/dbm.hpp"
using std::map;
using std::string;
using std::list;
using namespace newdbm;


namespace LTA {
  class Location;
  class Edge {
  private:
    string reset;
    list<dc_t*>* constraints;
    vector<int> resetVector;
    DBM dbm;
    Location* nextLocation;
    string id;
    bool isLoop;
    Location* prev;
  public:
    Edge (const string&, list<dc_t*>* , Location*,string, map<string,int>);
    Edge (const string&,  list<dc_t*>* ,string, map<string,int>);
    ~Edge ();

    void setLoop ();
    void setNextLocation (Location*);
    Location* getNextLocation ();
    const Location* getNextLocation () const;
    const string& getReset () const ;

    const vector<int>& getResetVector() const;

    const list<dc_t*>* getGuard () const ;
    string getGuardText () const;
    string getId () const ;
    const DBM& getDBM () const ;
    void setPrev(Location*);
    Location* getPrev ();
    bool isLoopTrans ();
  };

}

#endif

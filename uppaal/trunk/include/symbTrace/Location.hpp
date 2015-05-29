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
#ifndef _Location__
#define _Location__

#include <string>
#include <list>
#include "include/symbTrace/DCConstraint.hpp"
#include "include/rationalDBM/dbm.hpp"

using namespace newdbm;

using std::string;
using std::list;



namespace LTA { 
  
  
  class Edge;

  class Location {
  private:
    const list<dc_t*>* inv;
    Edge* edge;
    string id;
    DBM dbm;
    bool isLoop;
    Edge* back;
    Edge* loopBack;
  public:
    Location (const list<dc_t*>*, Edge*); 
    Location (const list<dc_t*>*);
    ~Location ();
    void setLoop ();
    bool isLoopLoc ();
    Edge* getEdge ();
    const Edge* getEdge () const;
    void setEdge (Edge*);
    const list<dc_t*>* getInvariant () const;
    string getInvariantText () const;
   
    void setDBM (DBM);
    const DBM& getDBM () const ;
    
    void setBack (Edge*);
    void setBackLoop (Edge*); 
    Edge* getBack ();
    Edge* getBackLoop ();
  };
}
#endif
  

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
#ifndef _NODE__
#define _NODE__



#include <string>

using std::string;

namespace LTA { 
  
  
  class Edge;

  class Node {
  private:
    string invariant;
    Edge* edge;
  public:
    Node (const string&, Edge*); 
    Node (const string&);
    ~Node ();
    Edge* getEdge ();
    const Edge* getEdge () const;
    void setEdge (Edge*);
   
  };
}

#endif
  
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

#ifndef _DBM__
#define _DBM__

#include <map>
#include <vector>
#include <set>
#include <string>
#include <ostream>
#include "realDBM/bound.hpp"

using std::map;
using std::set;
using std::string;
using std::ostream;
using std::vector;
typedef map<int,mpq_class> VariableAssignment;

class DBM {
private:
  string zeroClock;
  set<string> variablesOver;
  //map<string,map<string,Bound> > theDBM;
  vector<vector<Bound> > theDBM;
  bool keepClosed;
  bool inconsistent;
  void closeAfterUpdate ();
  void doClose ();

  int numVars;
  void zeroDiagonal ();
public:
  DBM () {}
  //DBM (string);
  DBM (int);
  DBM (VariableAssignment& );
  int numOfVars () const;
  bool hasSolution();
  //void setZero (string s);
  //string getZero ();
  void addVariable (string name);
  void constrain (int, int, Bound );
  void setConstraint (int, int, Bound );
  void setVar (int, mpq_class);
  bool contained (VariableAssignment&);
  void output (ostream& out,map<string,int>);

  Bound getBound (int, int) const;
  //
  void close (bool);
  //We assume the other DBM is over the same variables
  //as we are
  DBM intersect (DBM& );
  DBM reset (vector<int>&);
  DBM up ();
  DBM down ();
  DBM negReset (vector<int>&);
  bool subset (DBM& b);

  VariableAssignment findPoint ();
  VariableAssignment findMaxPoint ();
  VariableAssignment findPointMinClock (int , mpq_class );
};

#endif

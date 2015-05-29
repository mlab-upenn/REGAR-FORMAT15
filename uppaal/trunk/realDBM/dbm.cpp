
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

#include <vector>

#include "realDBM/dbm.hpp"
#include "realDBM/bound.hpp"

#include <gmpxx.h>

#include <iostream>

using std::vector;
using std::cout;
using std::endl;


DBM::DBM (VariableAssignment& v) :  theDBM (v.size(), vector<Bound> (v.size ())) {
  numVars = v.size();
  zeroDiagonal ();
 VariableAssignment::iterator it;
  for (it = v.begin ();it!=v.end ();it++) {
    Bound b ((*it).second,false);
    Bound b2 (-1*(*it).second,false);
    setConstraint ((*it).first,0,b);
    setConstraint (0,(*it).first,b2);
  }
  
  doClose ();
}

DBM::DBM (int num) :  theDBM (num, vector<Bound> (num)) {
  numVars = num;
  zeroDiagonal ();
}

void DBM::output (ostream& o, map<string,int> clocks) {
  map<string,int>::iterator i;
  map<string,int>::iterator j;

  for (i=clocks. begin ();i!=clocks.end ();i++) {
     for (j=clocks. begin ();j!=clocks.end ();j++) {
       if (getBound ((*i).second,(*j).second).isUnbound ()) {
	 o << (*i).first << " -" << (*j).first << "<" << "inf" << endl;
       }
       else {
	 o << (*i).first << " -" << (*j).first<< ((theDBM[(*i).second][(*j).second].isStrict ()) ? ("< ") : ("<=") ) <<  (theDBM[(*i).second][(*j).second].getBound ()) << endl;
       }
     }
  }

  cout << endl << endl;
}

void DBM::zeroDiagonal () {
  Bound b (mpq_class(0),false);
  for (int i = 0; i<numOfVars ();i++) {
    theDBM[i][i] = b;
  }
}

void DBM::constrain (int  x, int y, Bound b) {
  int i;
  int j;
  if (b < theDBM[x][y]) {
    setConstraint(x,y, b);
    for (i = 0; i != numVars; i++) {
      for (j = 0; j != numVars; j++) {
	Bound buf = theDBM[i][x] + theDBM[x][j];
	if (buf < theDBM [i][j]) {
	  theDBM[i][j] = buf;
	}

	buf = theDBM[i][y] + theDBM[y][j];
	if (buf < theDBM [i][j]) {
	  theDBM[i][j] = buf;
	}
      }
      }
  }


}

void DBM::setConstraint (int firstVar, int secondVar, Bound b) {

  theDBM [firstVar][secondVar] = b;
}

bool DBM::contained (VariableAssignment& assignment) {
  int xit;
  int yit;
  for (xit = 0; xit != numVars; xit++) {
    for (yit = 0; yit != numVars; yit++) {
      mpq_class value = assignment[xit] - assignment[yit];
      if (!theDBM[xit][yit].satisfied (value))
	return false;
    }
  }

  return true;
}

void DBM::closeAfterUpdate () {
  if (keepClosed)
    doClose ();
}
//Foyd_Warshall shortest path algorithm
void DBM::doClose () {
  int i;
  int j;
  int k;
  Bound zeroB (mpq_class (0),false);
  for (k = 0; k <numVars; k++) {
    for (i = 0; i <numVars; i++) {
       for (j = 0; j <numVars; j++) {
	 if (i!=j) {
	   Bound orig = theDBM [i][j];
	   Bound sum =  theDBM [i][k] + theDBM[k][j];
	   if (sum < orig) {

	       theDBM [i][j] = sum;

	   }

	 }
       }
    }
  }
}

void DBM::close (bool keep) {
  keepClosed = true;
  doClose ();
}

Bound DBM::getBound (int x, int y) const {
  Bound b =  theDBM[x][y];
  return b;
}

DBM DBM::up () {
  DBM newDBM (*this);
  Bound b;
  int i;

  //newDBM.setZero (zeroClock);
  for (i = 1; i != numVars; i++) {
    //if (*i!=zeroClock)
      newDBM.setConstraint (i,0,b);
  }
  return newDBM;
}



DBM DBM::down () {
  DBM newDBM (*this);
  Bound zero (mpq_class(0),false);
  int i;
  int j;

  for (i = 1; i != numVars; i++) {
    
    newDBM.setConstraint (0,i,zero);
    
    for (j = 0; j != numVars; j++) {
      Bound ji = getBound (j,i);
      Bound oi = getBound (0,i);
      if (ji < oi)
	newDBM.setConstraint (0,i,ji);
    }
   
  }
  //newDBM.close (true);
  return newDBM;
}

DBM DBM::reset (vector<int>& vars) {
  DBM newDBM (*this);
  //Bound zero (mpq_class(0),false);
  //Bound inf;
  mpq_class zero (0);
  int xit;
  vector<int>::iterator resetIt;
  
  for (resetIt = vars.begin (); resetIt != vars.end (); resetIt++) {
    /*
    for (xit = 0; xit != numVars; xit++) {
      Bound rx = getBound (0,xit);
      Bound xr = getBound (xit,0);
      
      newDBM.setConstraint (*resetIt,xit,rx);
      newDBM.setConstraint (xit,*resetIt,xr);
      
      }*/
    
    newDBM.setVar (*resetIt,mpq_class (0));
  }
  
  
  //newDBM.close (true);
  
  
  return newDBM;
  
}
DBM DBM::negReset (vector<int>& vars) {
  DBM newDBM (*this);
  Bound b;
  int i;
  vector<int>::iterator f;
  for (f = vars.begin ();f!=vars.end ();f++) {

    for (i = 0;i!=numVars;i++) {
      if (i!=*f) {
	newDBM.setConstraint (*f,i,b);
	newDBM.setConstraint (i,*f,getBound (i,0));

      }
    }

  }
  //newDBM.close (true);



  return newDBM;
}

//
DBM DBM::intersect (DBM& other) {

  DBM newDBM (*this);
  int i;
  int j;
  for (i = 0; i < numVars; i++) {
    for (j = 0; j < numVars; j++) {
      Bound oth = other.getBound (i,j);
      
      if (oth <  (theDBM[i][j]))
	
	newDBM.constrain (i,j,oth);
      
    }
  }
  newDBM.close (true);
  return newDBM;
}

//Pre condition: consistent DBM
VariableAssignment DBM::findPoint () {
  vector<int> boundVars (theDBM.size ());
  VariableAssignment sol;

  vector<int>::iterator boundIt;
  int unBoundIt;

  sol[0] = mpq_class  (0);
  boundVars.push_back (0);

  for (unBoundIt = 1;unBoundIt != numVars;unBoundIt++) {

    Bound upper = getBound (unBoundIt,0);
    Bound lower = getBound (0,unBoundIt);

    for (boundIt = boundVars.begin ();boundIt != boundVars.end ();boundIt++) {
      Bound buf =  getBound (unBoundIt,*boundIt);


      Bound upBound = Bound (sol[*boundIt],false) + buf;
      upper = (upBound < upper) ? upBound : upper;
      buf = getBound (*boundIt,unBoundIt);
      Bound lowBound = Bound (-1*sol[*boundIt],false) + buf;

      lower = (lowBound < lower) ? lowBound : lower;
    }

    //We will (for now)  just choose a point right between upper and lower

    if (!lower.isStrict ()) {

      sol[unBoundIt] =  -1*lower.getBound ();
    }

    else if (upper.isUnbound ()) {
      sol[unBoundIt] = (0-lower.getBound ()+1);
      }
    else {

      sol[unBoundIt] = (upper.getBound () - lower.getBound ()) / 2;
    }

    boundVars.push_back(unBoundIt);
  }

  return sol;

}

VariableAssignment DBM::findMaxPoint () {
  vector<int> boundVars (theDBM.size ());
  VariableAssignment sol;

  vector<int>::iterator boundIt;
  int unBoundIt;

  sol[0] = mpq_class  (0);
  boundVars.push_back (0);

  for (unBoundIt = 1;unBoundIt != numVars;unBoundIt++) {

    Bound upper = getBound (unBoundIt,0);
    Bound lower = getBound (0,unBoundIt);

    for (boundIt = boundVars.begin ();boundIt != boundVars.end ();boundIt++) {
      Bound buf =  getBound (unBoundIt,*boundIt);


      Bound upBound = Bound (sol[*boundIt],false) + buf;
      upper = (upBound < upper) ? upBound : upper;
      buf = getBound (*boundIt,unBoundIt);
      Bound lowBound = Bound (-1*sol[*boundIt],false) + buf;

      lower = (lowBound < lower) ? lowBound : lower;
    }

    //We will (for now)  just choose a point right between upper and lower

    if (!upper.isStrict ()) {
    
      sol[unBoundIt] = upper.getBound ();
    }
    
    else if (!lower.isStrict ()) {
      
      sol[unBoundIt] =  -1*lower.getBound ();
    }

    else if (upper.isUnbound ()) {
      sol[unBoundIt] = (0-lower.getBound ()+1);
      }
    else {

      sol[unBoundIt] = (upper.getBound () - lower.getBound ()) / 2;
    }

    boundVars.push_back(unBoundIt);
  }

  return sol;

}

mpq_class chooseMinVal (Bound& up, Bound& low,mpq_class& eps) {
  if (!low.isStrict ())  {
    return 0-low.getBound ();
  }

  else if (low.isUnbound () && up.isUnbound ()) {
    cout << "Choosing zero val" << endl;
    return mpq_class (0);
  }

  else if (up.isUnbound ())
    return 0-low.getBound ()+eps;

  else if (0-low.getBound () + eps < up.getBound ()) {
    return 0-low.getBound () + eps;
  }

  else {
    return((up.getBound () - low.getBound ())/2);
  }

}

void DBM::setVar (int var, mpq_class value) {
  Bound b1  (value,false);
  Bound b2 (-1*value,false);

  for (int i = 0; i< numVars;i++) {
    //theDBM[var][i] = 
    Bound oi = getBound (0,i);
    setConstraint (var,i,b1 + oi);
    //theDBM[i][var] = 
    Bound io = getBound (i,0);
    setConstraint(i,var,b2 + io);
  }
}

VariableAssignment DBM::findPointMinClock (int minClock, mpq_class eps ) {
  vector<int> boundVars (theDBM.size ());
  VariableAssignment sol ;

  vector<int>::iterator boundIt;
  int unBoundIt;


  sol[0] = mpq_class  (0);
  boundVars.push_back (0);

  Bound up = getBound (minClock,0);
  Bound down =getBound (0,minClock);

  sol[minClock] = chooseMinVal (up,down,eps);
  boundVars.push_back (minClock);


  for (unBoundIt = 1;unBoundIt != numVars;unBoundIt++) {
    if (unBoundIt == minClock)
      continue;
    Bound upper = getBound (unBoundIt,0);
    Bound lower = getBound (0,unBoundIt);

    for (boundIt = boundVars.begin ();boundIt != boundVars.end ();boundIt++) {
      Bound buf =  getBound (unBoundIt,*boundIt);
      Bound upBound = Bound (sol[*boundIt],false) + buf;
      upper = (upBound < upper) ? upBound : upper;
      buf = getBound (*boundIt,unBoundIt);
      Bound lowBound = Bound (-1*sol[*boundIt],false) + buf;

      lower = (lowBound < lower) ? lowBound : lower;
    }

    //We will (for now)  just choose a point right between upper and lower

    if (!lower.isStrict ()) {

      sol[unBoundIt] =  -1*lower.getBound ();
    }


    else if (lower.isUnbound () && upper.isUnbound ())
      sol[unBoundIt] = mpq_class (0);

    else if (upper.isUnbound ()) {
      sol[unBoundIt] = (0-lower.getBound ()+1);
    }
    else {

      sol[unBoundIt] = (upper.getBound () - lower.getBound ()) / 2;

    }

    boundVars.push_back(unBoundIt);
  }

  return sol;


}

bool DBM::hasSolution () {
  if (!keepClosed)
    doClose ();
  return !inconsistent;
}

int DBM::numOfVars () const {
  return numVars;
}

bool DBM::subset (DBM& b) {
  for (int i =0;i<numOfVars ();i++) {
     for (int j =0;j<numOfVars ();i++) {
       if (i!=j) {
	 Bound his = b.getBound (i,j); 
	 if(getBound (i,j) > his)
	   return false;
	 
       }
     }
  }
  
  return true;

}

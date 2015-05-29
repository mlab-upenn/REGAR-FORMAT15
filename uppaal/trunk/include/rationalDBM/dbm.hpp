#ifndef _NEWDBM__
#define _NEWDBM__

#include <vector>
#include "gmpxx.h"
#include "include/rationalDBM/bound.hpp"
using std::vector;

typedef vector<mpq_class>  VariableAssignment;

namespace newdbm {
  
  //A classic DBM
  //Except for setConstraint, constrain and setVar all operations preserve the DBMs solution space and creates new DBM
  //index 0 is always the zero clock
  class DBM {
  private:
    vector< vector<Bound > > bounds;
    //sets the lower bounds on all clocks to (0,<=)
    void initLower ();
    //Set the upper bounds on all clocks to (0,<=)
    void initUpper ();
    //set all constraints to (0,<=) i.e. the init zone
    void initAll ();
    //Zeros the diagonal
    void setDiagonal ();
    //Reset one clock...Changes solution space
    void reset (int) ;
    //Free one clock...Changes solution space
    void free (int);
    //algorithm that that finds tighest constraints on clockToset based on the values from the variables in restricted
    void findPointCore (int clockToSet,Bound& upper, Bound&lower, vector<int>& restricted, VariableAssignment& assigned) const;
  public:
    //Create initial zone DBM with i clocks
    static DBM initZone (int i);
    //Create DBM with one variableAssignment in its solution space
    DBM (VariableAssignment);
    //Create DBM with a specific number of clocks..including zero clock
    DBM (int size = 1);
    //operator for notational convenience
    Bound operator() (int,int) const;
    Bound getBound (int ,int) const ;
    //Close DBM using floyd warshall algorith
    void close ();
    //Find intersection between this DBM and the argument oth
    DBM intersect (const DBM&) const;
   
    //Reset a set of clocks
    DBM reset (const vector<int>&) const; 
    
    //free the clocks
    DBM free (const vector<int>&) const ;
    //Classic DOwn operation
    DBM down () const;
    //Classic up operation
    DBM up ()const ;
    //Constrain the DBM
    void constrain (int,int,Bound) ;
    //Set the constraint...but do not preserve closed form 
    void setConstraint (int,int,Bound);
    //constrain a variable to a specific value
    void setVar (int,mpq_class);
    
    //Is this DBM a subset of the ohter
    bool subset (DBM) const;
    bool consistent () const;
    int numOfVars () const;
     
    //Containement checker
    bool contained (const VariableAssignment& val) const;

    VariableAssignment findPoint (mpq_class epsilon = mpq_class (0.1)) const;
    VariableAssignment findPointMax (mpq_class epsilon = mpq_class (0.1)) const;
    VariableAssignment findPointMin (mpq_class epsilon = mpq_class (0.1)) const;
    VariableAssignment findPointMinClock (int, mpq_class epsilon = mpq_class(0.1)) const;
  };

  ostream& operator<< (ostream& o, const DBM& dbm);
}


#endif


#ifndef _UDBM__
#define _UDBM__

#include <map>
#include <vector>
#include <string>
#include <ostream> 


#include "udbmwrap/bound.hpp"
#include "dbm/fed.h"
#include "dbm/dbm.h"
#include "dbm/constraints.h"

using namespace dbm;

using std::vector;
namespace udbmw {
  typedef vector<int> VariableAssignment;
  
  class DBM {
  private:
    dbm_t theDBM;
    public:
    DBM () {}
    DBM (int); 
    DBM (VariableAssignment& );
    int numOfVars ();
    bool hasSolution();
    void constrain (int, int, Bound );
    void setConstraint (int, int, Bound );
    void setVar (int, int);
    bool contained (VariableAssignment);
    //void output (ostream& out,map<string,int>);
    Bound getBound (int, int);
    //
    void close ( );
    //We assume the other DBM is over the same variables 
    //as we are
    DBM intersect (DBM& );
    DBM reset (vector<int>&);
    DBM up ();
    DBM down ();
    DBM negReset (vector<int>&);
    VariableAssignment findPoint ();
  };
}

#endif

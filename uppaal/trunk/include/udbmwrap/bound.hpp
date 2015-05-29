#ifndef _UBOUND__
#define _UBOUND__


#include "dbm/fed.h"
#include "dbm/dbm.h"
#include "dbm/constraints.h"

namespace udbmw {

  class Bound {
  private:
    raw_t raw;
  public:
    Bound (raw_t);
    Bound (int, bool);
    bool isUnbound ();
    bool isStrict ();
    int getBound ();
    
    bool operator< (Bound& );
    bool operator== (Bound& );
    bool operator> (Bound& );
    bool operator<= (Bound& );
    bool operator>= (Bound& );
    
    Bound operator+ (Bound&);
    bool isNeg ();
    bool satisfied (int);
    
  };
}
#endif

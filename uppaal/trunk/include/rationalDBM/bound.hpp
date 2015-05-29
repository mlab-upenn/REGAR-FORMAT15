#ifndef _NEWBOUND__
#define _NEWBOUND__

#include <iostream>
#include <gmpxx.h>


using std::ostream;


namespace newdbm {
  
  

  class Bound {
  private:
    bool ubound;
    mpq_class bound;
    bool strict;
  public:
    Bound (mpq_class,bool);
    Bound ();
    
    bool operator< (const Bound&) ;
    bool operator<= (const Bound&);
    bool operator== (const Bound&);
    bool operator> (const Bound&);
    bool operator>= (const Bound&);
    bool operator!= (const Bound&);
    
    Bound operator+ (const Bound&);
    mpq_class getBound() const ;
    bool isStrict () const;
    bool isUnbound () const ;
    bool satisfied (const mpq_class&) const;
    static mpq_class chooseValueMax (const Bound&  upper, const Bound& lower, const mpq_class& epsilon);
    static mpq_class chooseValueMin (const Bound&  upper, const Bound& lower, const mpq_class& epsilon);
    
    static mpq_class chooseValueBetween (const Bound& upper, const Bound& lower, const mpq_class& epsilon);
    
  };

  ostream& operator<< (ostream&,const Bound&);
}


#endif

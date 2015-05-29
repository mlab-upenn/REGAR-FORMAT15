#include <gmpxx.h>
#include "rationalDBM/bound.hpp"

#include <iostream>

using std::cerr;
using std::endl;
using std::ostream;

namespace newdbm {
  ostream& operator<< (ostream& o,const Bound& b) {
    if (b.isUnbound ())
      o << "(inf, <)" ;
    else {
      o << "(" << b.getBound () << (b.isStrict() ? "<" :  "<=") << ")";
    }
    return o;
  }

  Bound::Bound () {
    strict = true;
    ubound = true;
  }

  Bound::Bound (mpq_class b,bool s) {
    bound = b;
    strict  = s;
    ubound = false;
  }

  bool Bound::isStrict ()const {return strict;}

  mpq_class Bound::getBound () const  {
    return bound;
  }

  bool Bound::isUnbound ()const {
    return ubound;
  }

  bool Bound::operator== (const Bound& oth) {
    if (oth.isUnbound () && isUnbound ())
      return true;
    else
      return (oth.getBound () == getBound () &&
	      oth.isStrict () == isStrict ());

  }

  bool Bound::operator< (const Bound& oth) {
    if (isUnbound ()){ // (inf,<) (?,?){
      //cerr << *this <<" >= " << oth << endl;

      return false;
    }
    if (!isUnbound () && oth.isUnbound ()){ //(m,<) < (inf,<) {
      //cerr << *this << "< " << oth <<endl;
       return true;
    }
    else if (getBound () < oth.getBound()){ // (m,<) < (n,<)  where  m<n
      //cerr << *this << "< " << oth <<endl;
      return true;
    }
    else if (getBound() == oth.getBound () && isStrict () && !oth.isStrict ()) {//(m,<) && (m,<=) {
      //cerr << *this << "< " << oth <<endl;
      return true;
    }
    //cerr << *this <<" >= " << oth << endl;
    return false;
  }

  bool Bound::operator<= (const Bound& oth) {
    return (operator==(oth) || operator<(oth));
  }

  bool Bound::operator> (const Bound& oth) {
    return (!operator<= (oth));
  }

  bool Bound::operator>= (const Bound& oth) {
    return (operator==(oth) || operator>(oth));
  }

  bool Bound::operator!= (const Bound& oth) {
    return (!operator==(oth));
  }

  Bound Bound::operator+ (const Bound& oth) {
    Bound b;
    if (isUnbound () || oth.isUnbound ()) {
      //cerr << *this << "+" << oth << "=" << b << endl;
      return b;
    }
    bool strict =  (isStrict () || oth.isStrict ());
    Bound r (oth.getBound ()+getBound (),strict);
    //cerr << *this << "+" << oth << "=" << r << endl;
    return r;
  }

  mpq_class Bound::chooseValueBetween  (const Bound& upper, const Bound& lower, const mpq_class& epsilon) {


    //cerr << "CHoose value: " << upper << " -  " << lower << endl;

    if (upper.isUnbound () && lower.isUnbound ()) {
      cerr << "Strange: chooseValueBetween was called with two infinity bounds..." << endl;
      cerr << "Returning zero" << endl;
      return mpq_class (0);
    }

    else if (upper.isUnbound ()) {
      return chooseValueMin (upper,lower,epsilon);
    }

    else if  (lower.isUnbound ()) {

      return (chooseValueMax (upper,lower,epsilon));
    }

    else if (!lower.isStrict ()) {
      //cerr << "chose " << -1*lower.getBound () << endl << "lower";;
      return -1*lower.getBound ();
    }

    else if (!upper.isStrict ()) {
      //cerr << "chose " << lower.getBound ()<<endl<<"upper";
      return upper.getBound();
    }



    else {
      //cerr << "Choosing value between" << endl;
      return ((upper.getBound ()-lower.getBound ())/2);
    }
  }

  mpq_class Bound::chooseValueMin (const Bound& upper, const Bound& lower, const mpq_class& epsilon) {
    if (lower.isUnbound ()) {
     cerr << "Very strange: chooseValueMin was called with an infinity bound as lower..." << endl;
     cerr << "Returning zero" << endl;
      return mpq_class (0);
    }

    else if (lower.isStrict ()) {
      //cerr << "chose " << ((-1*lower.getBound ())+epsilon) << endl;
      return ((-1*lower.getBound ())+epsilon);
    }

    else {
      //cerr << "chose " <<  (-1*lower.getBound ()) << endl;
      return -1*lower.getBound ();
    }

  }

  mpq_class Bound::chooseValueMax (const Bound& upper, const Bound& lower, const mpq_class& epsilon) {
    if (upper.isUnbound ())
      {
	cerr << "chooseValueMax was called with an upper infinity bound. THis should not happen..." << endl;
	cerr << "uses chooseValueMin instead" << endl;
	chooseValueMin (upper,lower,epsilon);
      }
    else if (upper.isStrict ()) {
      //cerr << "chose " <<  upper.getBound ()-epsilon << endl;
      return upper.getBound ()-epsilon;
    }

    else {
      //cerr << "chose " <<  upper.getBound () << endl;
      return upper.getBound ();
    }
  }
  
  bool Bound::satisfied (const mpq_class& val) const {
    if (isUnbound ()) 
      return true;
    else if (val < getBound ()) 
      return true;
    else if (val == getBound () && !isStrict ()) 
      return true;
    else 
      return false;
  }

}

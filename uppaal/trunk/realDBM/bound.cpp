
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


#include "realDBM/bound.hpp"

#include <gmpxx.h>

//testing
#include <iostream>
using namespace std;

Bound::Bound (mpq_class b, bool str) : bound (b) {
  strict = str;
  unbounded = false;
}

Bound::Bound () {
  unbounded = true;
}

bool Bound::isStrict () {
  return strict;
}

mpq_class& Bound::getBound () {
  return bound;
}

bool Bound::operator== (Bound& other) {
  if (isUnbound () && other.isUnbound ()) 
    return true;
  return (other.getBound () == getBound () && isStrict () == other.isStrict ());
}


ostream& operator<< (ostream& o,Bound& b) {
  if (b.isUnbound ()) 
    o << "(inf, <)" ;
  else {
    o << "(" << b.getBound () << (b.isStrict() ? "<" :  "<=") << ")";
  }
}

bool Bound::operator<= (Bound& other) {
  if (other.isUnbound ()) {
    //cout << *this << " <= " << other << endl;
    return true;
  }
  else if (isUnbound()) {
    //cout << other << " <= " << *this << endl;
    return false;
  }
  else if (getBound () < other.getBound ()) {
    //cout << *this << " <=" << other << endl;
    return true;
  }
  else if (getBound () == other.getBound () && isStrict () && !other.isStrict ()) {
    //cout << *this << " <= " << other << endl;
    return true;
  }
  
   else { 
     //cout << other << " <= " << *this << endl;
      return false;
   }
}

bool Bound::operator< (Bound& other) {
  //return ((*this< other)) && !(*this == other));
  return operator<=(other) && !operator==(other);
}

bool Bound::operator>= (Bound& other) {
  return !operator< (other);;
}

bool Bound::operator> (Bound& other) {
  //return (!(*this< other) && !(*this==other));
  return !operator<= (other); 
}

Bound Bound::operator+ (Bound& other) {
  //cout << *this << "+" << other << "=" ;
  Bound b;
  if (other.isUnbound () || isUnbound ()) {
    //cout << b << endl;
    return b;
  }
  mpq_class newBound (other.getBound () + getBound ());
  bool strict = other.isStrict () || isStrict ();
  Bound h (newBound,strict); 
  //cout << h << endl;;
  return Bound (newBound,strict);
}

bool Bound::isNeg () {
  if (unbounded) 
    return false;
  if (getBound () < 0)
    return true;
  else if (getBound () == 0 && isStrict ()) 
    return true;
  else 
    return false;
}

bool Bound::isUnbound () {
  return unbounded;
}

bool Bound::satisfied (mpq_class& value) {
  if (isUnbound ()) 
    return true;
  if (value == getBound ())
    return (!isStrict ());
  else if (value < getBound ()) 
    return true;
  else 
    return false;
}
  

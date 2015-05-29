
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


#ifndef _BOUND__
#define _BOUND__

#include <gmpxx.h>
#include <ostream>

using std::ostream;

class Bound {
private:
  mpq_class bound;
  bool unbounded; 
  bool strict;
public:
  Bound (mpq_class, bool);
  Bound ();
  bool operator< (Bound& );
  bool operator== (Bound& );
  bool operator> (Bound& );
  bool operator<= (Bound& );
  bool operator>= (Bound& );
  mpq_class& getBound ();
  bool isStrict ();
  Bound operator+ (Bound&);
  bool isNeg ();
  bool isUnbound ();
  bool satisfied (mpq_class&);

};

  ostream& operator<< (ostream&, Bound& b);
#endif

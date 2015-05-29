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
#ifndef _DCCON__
#define _DCCON__

#include <string>
#include <iostream>

using std::ostream;
using std::string;

enum DCOperator {lt,leq,eq};

class dc_t
{
public:
  //dc_t(string&);
  dc_t(string,string,DCOperator,int);
  dc_t(const dc_t*);
  void setOp(DCOperator);
  DCOperator getOp() const;
  void replaceX(string);
  string getX() const;
  void replaceY(string);
  string getY() const;
  int getBound() const;
  friend ostream& operator<< (ostream& out, const dc_t&);

private:
  DCOperator op;
  string origX;
  string origY;
  string clockX;
  string clockY;
  int bound;
};


#endif

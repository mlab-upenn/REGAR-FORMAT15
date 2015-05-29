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
#include "symbTrace/DCConstraint.hpp"
#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::cout;

dc_t::dc_t(string x, string y, DCOperator  m,int b) {
  op =m;
  bound = b;
  clockX = x;
  clockY = y;
  origX = x;
  origY = y;
}

dc_t::dc_t(const dc_t* constraint) {
  op =constraint->getOp();
  bound = constraint->getBound();
  clockX = constraint->getX();
  clockY = constraint->getY();
  origX = "";
  origX ="";

}


DCOperator dc_t::getOp () const {
  return op;
}


void dc_t::replaceX (string r) {
  origX = clockX;
  clockX = r;
}

string dc_t::getX () const{
    return clockX;
  //return string (" ");
}

void dc_t::replaceY (string r) {
  origY = clockY;
  clockY = r;
}

string dc_t::getY () const{
  
  return clockY;
}

int dc_t::getBound () const{
  return bound;
}


ostream& operator<< (ostream& out,const dc_t& d) {
  string comp;
  switch (d.op) {
  case lt:
    comp = string ("<");
    break;
  case leq:
    comp = string ("<=");
    break;
  default:
    comp = string("==");
  }
  out << d.clockX <<"-" << d.clockY << comp <<d. bound << "was "  << d.origX <<"-" << d.origY << comp <<d. bound;   
  
  return out;
}

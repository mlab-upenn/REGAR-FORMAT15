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
#include "xmlloader/Visitable.hpp"
#include <sstream>
#include <iostream>
#include "xmlloader/Visitor.hpp"

using std::cout;
using std::endl;
using std::ostringstream;

Guard::Guard (bool gre, bool str,Expr* l,Expr* r) {
  greater = gre;
  strict = str;
  equal = false;
  left = l;
  right = r;
}

Guard::Guard (Expr* l, Expr* r) {
  greater = false;
  strict =  false;
  left  = l;
  right = r;
}

void Guard::SwapLeftAndRight () {
  Expr* buf = left;
  left = right;
  right = buf;
}

void Guard::visit (Visitor *v) {
  v->caseGuard (this);
}

ParanExpr::ParanExpr (Expr* e ) {
  inParan = e;
}


NegExpr::NegExpr (Expr* e) {
  negated = e;
}

PlusExpr::PlusExpr (Expr* l, Expr* r ) {
  left = l;
  right = r;
}


SubExpr::SubExpr (Expr* l, Expr* r ) {
  left = l;
    right = r;
}


MultExpr::MultExpr (Expr* l, Expr* r ) {
  left = l;
  right = r;
}


DivExpr::DivExpr (Expr* l, Expr* r ) {
  left = l;
  right = r;
}


ModExpr::ModExpr (Expr* l, Expr* r ) {
  left = l;
  right = r;
}

IdentExpr::IdentExpr (string* id) {
  identifier = id;
}

ValExpr::ValExpr (int val) {

  value = val;
}


void ValExpr::toString (ostream& o){
  o << value;
}

void ValExpr::visit (Visitor* v) {
  v->caseValExpr (this);
}
int ValExpr::getValue () {return value;}

string IdentExpr::getIdentifier () {
  return string (*identifier);
}
void IdentExpr::toString (ostream& o){

  o<< getIdentifier ();
}


void IdentExpr::visit (Visitor* v) {
  v->caseIdentExpr (this);
}

Expr* ParanExpr::getParantheses () {
  return inParan;
}
void ParanExpr::visit (Visitor* v) {}
void ParanExpr::toString (ostream& o){
  o<< "(";
  inParan->toString (o);
  o<< string (")");
}

Expr* NegExpr::getNegate () {
  return negated;
}
void NegExpr::visit (Visitor* v) {}
void NegExpr::toString (ostream& o){
  o<< "-";
  negated->toString (o);
}



Expr* PlusExpr::getLeft () {return left;}
Expr* PlusExpr::getRight () {return right;}
void PlusExpr::visit (Visitor* v) {
  v->casePlusExpr (this);
}
void PlusExpr::toString (ostream& o){
  left->toString (o);
  o << "+";
  right->toString (o);

}

Expr* SubExpr::getLeft () {return left;}
Expr* SubExpr::getRight () {return right;}
void SubExpr::visit (Visitor* v) {
  v->caseSubExpr (this);
}
void SubExpr::toString (ostream& o){
    left->toString (o);
  o << "-";
   right->toString (o);
}

Expr* MultExpr::getLeft () {return left;}
Expr* MultExpr::getRight () {return right;}
void MultExpr::visit (Visitor* v) {
  v->caseMultExpr (this);
}
void MultExpr::toString (ostream& o){
  left->toString (o);
  o << "*";
   right->toString (o);
}

Expr* DivExpr::getLeft () {return left;}
Expr* DivExpr::getRight () {return right;}
void DivExpr::visit (Visitor* v) {
  v->caseDivExpr (this);
}
void DivExpr::toString (ostream& o){
  left->toString (o);
   o << "/";
  right->toString (o);
}


Expr* ModExpr::getLeft () {return left;}
Expr* ModExpr::getRight () {return right;}
void ModExpr::visit (Visitor* v) {
  v->caseModExpr (this);
}
void ModExpr::toString (ostream& o){
  left->toString (o);
   o << "%";
  right->toString (o);
}


void Guard::toString (ostream& o) {
  left->toString (o);
  o<< "<<";
  right ->toString (o);
  o << endl;
}

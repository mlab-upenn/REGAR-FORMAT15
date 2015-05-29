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

#ifndef _VISITABLE__
#define _VISITABLE__

#include <string>
#include <iostream>

using std::ostream;
using std::string;

class Visitor;

class Visitable {
public:
  virtual void visit (Visitor*) = 0;
  virtual void toString (ostream& ) = 0;
};

class Expr : public Visitable {
};

class StartExpr  : public Expr {
private:
  Expr* ex;
  
public: 
  StartExpr(Expr* e) {ex = e;}
  ~StartExpr () {delete ex;}
  Expr* getExpr () {return ex;}
  void visit (Visitor* visitor ) {
    ex->visit (visitor);
  }
  void toString (ostream& o) {
    ex->toString (o);
  }
};

class ValExpr : public Expr {
private:
  int value;
public:
  int getValue ();
  ValExpr (int val);

   void visit (Visitor*);
  virtual void toString (ostream&);
};


class IdentExpr : public Expr {
private:
  string* identifier;
public:
  IdentExpr (string* id);
  ~IdentExpr () {
    delete identifier;
  }
string getIdentifier ();
   void visit (Visitor*);
  virtual void toString (ostream&);
};


class ParanExpr : public Expr {
private:
  Expr* inParan;
public:
  ParanExpr (Expr*);
  ~ParanExpr () {
    delete inParan;
  }
  Expr* getParantheses ();
  void visit (Visitor*);
  virtual void toString (ostream&);
};

class NegExpr : public Expr {
private:

  Expr* negated;
public:
  NegExpr (Expr*);
  ~NegExpr () {
    delete negated;
  }
  Expr* getNegate ();
  void visit (Visitor*);
  virtual void toString (ostream&);
};



class PlusExpr : public Expr {
private:
  Expr* left;
  Expr* right;
public:
  PlusExpr (Expr*,Expr*);
  ~PlusExpr () {
    delete left;
    delete right;
  }
  Expr* getLeft ();
  Expr* getRight ();
  void visit (Visitor*);
  virtual void toString (ostream&);
};


class SubExpr : public Expr {
private:
  Expr* left;
  Expr* right;
public:
  SubExpr (Expr*,Expr*);
  ~SubExpr () {
    delete left;
    delete right;
  }
  Expr* getLeft ();
  Expr* getRight ();
  void visit (Visitor*);
  virtual void toString (ostream&);
};


class MultExpr : public Expr {
private:
  Expr* left;
  Expr* right;
public:
  MultExpr (Expr*,Expr*);
  ~MultExpr () {
    delete left;
    delete right;
  }
  Expr* getLeft ();
  Expr* getRight ();
   void visit (Visitor*);
  virtual void toString (ostream&);
};


class DivExpr : public Expr {
private:
  Expr* left;
  Expr* right;
public:
  DivExpr (Expr*,Expr*);
   ~DivExpr () {
    delete left;
    delete right;
  }
  Expr* getLeft ();
  Expr* getRight ();
   void visit (Visitor*);
  virtual void toString (ostream&);
};

class ModExpr : public Expr {
private:
  Expr* left;
  Expr* right;
public:
  ModExpr (Expr*,Expr*);
   ~ModExpr () {
    delete left;
    delete right;
  }
  Expr* getLeft ();
  Expr* getRight ();
  void visit (Visitor*);
  virtual void toString (ostream&);
};

class Guard  :public Visitable{
private:
  bool equal;
  bool greater;
  bool strict;
  Expr* left;
  Expr* right;
public:
  Guard (bool, bool, Expr*,Expr*);
  Guard ( Expr* ,Expr*);
  ~Guard () {
    delete left;
    delete right;
  }
  void SwapLeftAndRight();
  void visit (Visitor*);
  virtual void toString (ostream&);
  bool isStrict () {return strict;}
  bool isGreater () {return greater;}
  bool isEqual () {return equal;}
  Expr* getLeft () {return left;}
  Expr* getRight () {return right;}
  void setLeft (Expr* newLeft) {left = newLeft;}
  void setRight (Expr* newRight) {right = newRight;}
};

#endif

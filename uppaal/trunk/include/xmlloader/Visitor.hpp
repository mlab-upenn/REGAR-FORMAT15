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

#ifndef _VISITOR__
#define _VISITOR__


#include<list>
#include<string>
#include<map>
#include<vector>
#include "Visitable.hpp"
#include "symbTrace/DCConstraint.hpp"

using namespace std;

const bool plusSign = true;
const bool minusSign = false;

const bool leftSide = true;
const bool rightSide = false;


class Visitor {
public:
  virtual void casePlusExpr (PlusExpr* expr)=0;
  virtual void caseSubExpr (SubExpr* expr)=0;
  virtual void caseDivExpr (DivExpr* expr)=0;
  virtual void caseMultExpr (MultExpr* expr)=0;
  virtual void caseParanExpr (ParanExpr* expr)=0;
  virtual void caseNegExpr (NegExpr* expr )=0;
  virtual void caseValExpr (ValExpr* expr)=0;
  virtual void caseIdentExpr (IdentExpr* expr)=0;
  virtual void caseGuard (Guard* guard) = 0;
  virtual void caseModExpr (ModExpr* expr)=0; 
};

class ArithmeticEvaluator :public Visitor {
private: 
  int buffer;
  map<string,int>* varValues;
  map<string,string>* systemVariableNameToId;
  map<string,string>* localVariableNameToId;
public:
  int evalExpr (Expr* );
  void casePlusExpr (PlusExpr* expr);
  void caseSubExpr (SubExpr* expr);
  void caseDivExpr (DivExpr* expr);
  void caseMultExpr (MultExpr* expr);
  void caseParanExpr (ParanExpr* expr);
  void caseNegExpr (NegExpr* expr);
  void caseValExpr (ValExpr* expr);
  void caseIdentExpr (IdentExpr* expr);
  void caseGuard (Guard* guard);
  void caseModExpr (ModExpr* expr);
  void setValues (map<string,string>*,  map<string,string>*, map<string,int>*);
};


class ConstraintModifier :public Visitor {
private:
  bool whatSide;
  string clock1;
  string clock2;
  string comp;
  int rh;
  DCOperator op;

  vector<bool> signStack;
  vector<int> tempResults;

  map<string,int>* varValues;
  map<string,string>* systemVariableNameToId;
  map<string,string>* localVariableNameToId;
  map<string,string>* systemClockNameToId;
  map<string,string>* localClockNameToId;
  int value (string& ident);

public:
  ConstraintModifier (string zero) {
    signStack.push_back(plusSign);
    clock1 = zero;
    clock2 = zero;
  }
  void casePlusExpr (PlusExpr* expr);
  void caseSubExpr (SubExpr* expr);
  void caseDivExpr (DivExpr* expr);
  void caseMultExpr (MultExpr* expr);
  void caseParanExpr (ParanExpr* expr);
  void caseNegExpr (NegExpr* expr);
  void caseValExpr (ValExpr* expr);
  void caseIdentExpr (IdentExpr* expr);
  void caseGuard (Guard* guard);
  void caseModExpr (ModExpr* expr);
  
  dc_t* createGuard ();

  void setClocks (map<string,string>*,  map<string,string>*);
  void setValues (map<string,string>*,  map<string,string>*, map<string,int>*);
};
#endif


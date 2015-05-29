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
    We get an expression on the form: EXPR OPERATOR EXPR
    where EXPR is one of PLUS MINUS TIMES DIVIDE IDENT NUMBER, and
    OPERATOR is one of < <= == => >.

    A TIMES or DIVIDE expression is an expression which cannot contain a clock,
    thus we can always reduce this to being a number.
*/

#include<list>
#include<string>

#include<iostream>
#include<sstream>
#include<algorithm>

using namespace std;
#include "xmlloader/Visitor.hpp"
#include "xmlloader/Visitable.hpp"
#include "symbTrace/DCConstraint.hpp"




dc_t* ConstraintModifier::createGuard ()
{
    return new dc_t (clock1,clock2,op,rh);
}

void ConstraintModifier::caseGuard (Guard* guard)
{
    if (guard->isEqual())
        op=eq;
    else if (guard->isGreater())
    {
        guard->SwapLeftAndRight();
        if (guard->isStrict())
            op = lt;
        else
            op=leq;
    }
    else
    {
        if (guard->isStrict())
            op = lt;
        else
            op=leq;
    }

    whatSide = leftSide;
    guard->getLeft()->visit (this);
    int leftResult = tempResults.back();
    tempResults.pop_back();

    whatSide = rightSide;
    guard->getRight()->visit (this);
    int rightResult = tempResults.back();
    tempResults.pop_back();
    rh = rightResult-leftResult;
}


/* First checks whether the name is a local clock or variable,
 * and afterwards whether it is a global clock or variable.
 * In case it is a clock, it checks what side of the expression we
 * are on as well as the sign of the expression to see how it
 * fits into the form "clock1 - clock2 COMPARISON rh".
 * If it is a variable, we look up its value and add it to the stack.
 */

void ConstraintModifier::caseIdentExpr (IdentExpr* expr)
{
    string name = expr->getIdentifier ();
    string tempId, id;
    bool itsAClock = false;

    if ((tempId=(*(localClockNameToId))[name])!="")
    {
        id = tempId;
        itsAClock = true;
    }
    else if ((tempId=(*(localVariableNameToId))[name])!="")
    {
        id = tempId;
    }
    else if ((tempId=(*(systemClockNameToId))[name])!="")
    {
        id = tempId;
        itsAClock = true;
    }
    else if ((tempId=(*(systemVariableNameToId))[name])!="")
    {
        id = tempId;
    }
    else
    {
        cerr << "Name not identified as clock or variable." << endl;
        return;
    }


    if (itsAClock)
    {
        if (signStack.back() == plusSign)
        {
            if (whatSide==leftSide)
            {
                clock1 = id;
            }
            else //if(whatSide == rightSide)
            {
                clock2 = id;
            }
        }
        else // if(signStack.back() == minusSign)
        {
            if (whatSide==leftSide)
            {
                clock2 = id;
            }
            else //if(whatSide == rightSide)
            {
                clock1 = id;
            }
        }
        tempResults.push_back(0);
    }
    else // if(!itsAClock)    -- thus a variable
    {
        int value = (*(varValues))[id];
        tempResults.push_back(value);
    }
}


void ConstraintModifier::caseValExpr (ValExpr* expr)
{
    tempResults.push_back(expr->getValue());
}

void ConstraintModifier::caseNegExpr (NegExpr* expr)
{
    signStack.push_back(signStack.back());
    expr->visit(this);
    int a = tempResults.back();
    tempResults.pop_back();
    a *= -1;
    tempResults.push_back(a);
    signStack.pop_back();
}

void ConstraintModifier::caseParanExpr (ParanExpr* expr)
{
    expr->getParantheses ()->visit (this);
}

void ConstraintModifier::caseMultExpr(MultExpr* expr)
{
    int a, b, result;

    expr->getLeft()->visit (this);
    a = tempResults.back();
    tempResults.pop_back();
    expr->getRight()->visit (this);
    b = tempResults.back();
    tempResults.pop_back();
    result = a * b;
    tempResults.push_back(result);
}


// Let a + b be the expression. Using tempResult, the value of a is found,
// saved and afterwards the value of b is found. Then the value of a + b is
// pushed onto tempResult.
void ConstraintModifier::casePlusExpr(PlusExpr* expr)
{
    int a, b, result;

    expr->getLeft()->visit (this);
    a = tempResults.back();
    tempResults.pop_back();
    expr->getRight()->visit (this);
    b = tempResults.back();
    tempResults.pop_back();
    result = a + b;
    tempResults.push_back(result);
}


void ConstraintModifier::caseDivExpr(DivExpr* expr)
{
    int a, b, result=0;

    expr->getLeft()->visit (this);
    a = tempResults.back();
    tempResults.pop_back();
    expr->getRight()->visit (this);
    b = tempResults.back();
    tempResults.pop_back();
    if (b==0)
    {
        cerr << "Attempting to make zero-division in ConstraintModifier." << endl;
        return;
    }
    else
        result = a / b;
    tempResults.push_back(result);
}



void ConstraintModifier::caseModExpr(ModExpr* expr)
{
   
}


void ConstraintModifier::caseSubExpr(SubExpr* expr)
{
    int a, b, result;
    expr->getLeft()->visit (this);
    a = tempResults.back();
    tempResults.pop_back();

    signStack.push_back(!signStack.back());

    expr->getRight()->visit (this);
    b = tempResults.back();
    tempResults.pop_back();

    signStack.pop_back();

    result = a - b;
    tempResults.push_back(result);
}

void ConstraintModifier::setClocks (map<string,string>* systemClocks,  map<string,string>* localClocks)
{
    systemClockNameToId = systemClocks;
    localClockNameToId = localClocks;
}

void ConstraintModifier::setValues (map<string,string>* systemVariables,  map<string,string>* localVariables, map<string,int>* varState)
{
    systemVariableNameToId = systemVariables;
    localVariableNameToId = localVariables;
    varValues = varState;
}

void ArithmeticEvaluator::setValues (map<string,string>* systemVariables,  map<string,string>* localVariables, map<string,int>* varState)
{
    systemVariableNameToId = systemVariables;
    localVariableNameToId = localVariables;
    varValues = varState;
}

void ArithmeticEvaluator::casePlusExpr (PlusExpr* expr) {
  expr->getLeft ()->visit (this);
  int buf = buffer;
  expr->getRight ()->visit (this);
  buffer = buf+buffer;
}

void ArithmeticEvaluator::caseSubExpr (SubExpr* expr) {
  expr->getLeft ()->visit (this);
  int buf = buffer;
  expr->getRight ()->visit (this);
  buffer = buf-buffer;
}

void ArithmeticEvaluator::caseDivExpr (DivExpr* expr) {
  expr->getLeft ()->visit (this);
  int buf = buffer;
  expr->getRight ()->visit (this);
  buffer = buf/buffer;
}

void ArithmeticEvaluator::caseMultExpr (MultExpr* expr) {
  expr->getLeft ()->visit (this);
  int buf = buffer;
  expr->getRight ()->visit (this);
  buffer = buf*buffer;
}

void ArithmeticEvaluator::caseParanExpr (ParanExpr* expr) {
  expr->getParantheses ()->visit (this);
}

void ArithmeticEvaluator::caseNegExpr (NegExpr* expr) {
  expr->getNegate ()->visit (this);
  buffer = -1*buffer;
}

void ArithmeticEvaluator::caseValExpr (ValExpr* expr) {
  buffer = expr->getValue();
}

void ArithmeticEvaluator::caseIdentExpr (IdentExpr* expr) {
  //cout << "Identity" << endl;
  string id; 
  string name = expr->getIdentifier ();
  string tempId;
  if ((tempId=(*(localVariableNameToId))[name])!="")
    {
      id = tempId;
    }
  else if ((tempId=(*(systemVariableNameToId))[name])!="")
    {
      id = tempId;
    }

  else {
    cerr << "Sorry not a variable:  " <<  name <<  endl;
  }
  
  buffer = (*(varValues))[id];
}


int ArithmeticEvaluator::evalExpr (Expr* g) {
  //cout << "Starting" << endl;
  buffer = 0;
  g->visit (this);
  return buffer;
}

void ArithmeticEvaluator::caseGuard (Guard* g) {
  //Not used
}

void ArithmeticEvaluator::caseModExpr (ModExpr* expr) {
  expr->getLeft ()->visit (this);
  int buf = buffer;
  expr->getRight ()->visit (this);
  buffer = buf%buffer;
}

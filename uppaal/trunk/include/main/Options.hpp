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


#ifndef _OPTIONS__
#define _OPTIONS__


#include <gmpxx.h>
#include <string>
#include <map>
using std::map;
using std::string;
struct  Options{
  bool time; //do time measuring
  bool floating; //output result as floats 
  bool entry; //output entry times to screen instead of delays
  string modelpath; //path to model
  bool saveRes; //
  string saveResTo; //file to save concrete trace to ;
  string solver; //name of chosen solver
  mpz_class epsilon; //The epsilon  
  
  string solverPath; //Path to find dynamic solvers
  string zeroRep ; //ZeroRepresentation
  bool externalSolver;
  bool quiet;
  bool fast;
  string globalC;

  string solveOptions; //Options for the sovlers 

  map<string,int> clockIndex;
};


Options* loadOptions (int argc, char** argv);


#endif

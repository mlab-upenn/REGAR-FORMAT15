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
%option noyywrap
%option prefix="guard"
%{
#include<stdio.h>
#include<stdlib.h>
#include <string>
#include <iostream>

#include "xmlloader/bison.tab.hpp"
using std::string;
using std::cout;
using std::endl;
%}




number [0-9]+
identifier [a-zA-Z][a-zA-Z\_\[\]0-9\(\)]*

%%
{number} {
guardlval.value = atoi (yytext);return  NUMBER;}
{identifier} {
guardlval.text = new string (yytext);return IDENTIFIER;}
"+" return PLUS;
"*" return TIMES;
"/" return DIVIDE;
"-" return  MINUS;
"<" return LESS;
"<=" return LEQ;
">=" return GEQ;
">"  return GREATER;
"==" return EQUAL ;
"(" return LPARAN;
")" return RPARAN;
"!=" return NEQUAL;
%%

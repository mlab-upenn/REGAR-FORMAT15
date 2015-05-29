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
#ifndef _dccreator_
#define _dccreator_
#include "symbTrace/Location.hpp"
#include "symbTrace/Edge.hpp"
#include "symbTrace/lta.hpp"
#include "dbm/fed.h"
#include <map>
#include <vector>


using namespace std;
using namespace LTA;
using namespace dbm;
using std::map;

namespace DCCREATOR
{
	class DCCreator
	{

		public:
			DCCreator(Lta*, string&);
			~DCCreator();
			dbm_t& CreateEntryTimeConstraints();

        private:
            // comparison for strings
            struct strCmp
            {
                bool operator()(const string s1,const string s2 ) const
                {
                    const char *c1;
                    const char *c2;

                    c1=s1.c_str();
                    c2=s2.c_str();
                    return strcmp( c1, c2 ) < 0;
                }
            };

            dbm_t entryTimeDBM;
            string zeroClockRepresentation;
            map<string, int, strCmp> entrytimesmap;
            Lta* lta;


            int LastResetAt(int,string&);
            void CreateLastResetAtTable();

            int* lastresetdata;
            map<string, int, strCmp> clocknumbermap;
            void CreateConstraint(raw_t*, int, const list<dc_t*>*, int, bool);
            void constrainEntry(raw_t*, int, int, int, int, bool);
	};
}

#endif

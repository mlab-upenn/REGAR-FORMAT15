
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



#include <sstream>
#include <string>
#include <iostream>


#include "dlfcn.h"
#include <io.h>
#include "include/solver/interface.hpp"
#include "include/solver/interfaceStat.hpp"

#include "include/symbTrace/lta.hpp"
#include "include/symbTrace/Location.hpp"
#include "include/symbTrace/Edge.hpp"
#include "include/symbTrace/DCConstraint.hpp"
#include "include/xmlloader/XMLLoader.hpp"

#include "include/main/Options.hpp"



#include <libxml/parser.h>
#include <libxml/xmlsave.h>
#define standardSolver "point"

using namespace std;

void* handle;


solve_func solve_plug;

xmlDocPtr doc;

void getTrans ( map<string,xmlNodePtr>& theMap, xmlNodePtr  cur2)
{
    xmlNodePtr cur = cur2->xmlChildrenNode;
    while (cur!=0)
    {
        if (!xmlStrcmp (cur->name, (const xmlChar*)"transition"))
        {
            xmlChar* from = xmlGetProp (cur,(const xmlChar*) "from");
            if (from != 0)
            {
                string strFrom ((const char*) from);
                theMap[strFrom] = cur;
            }
        }
        else
        {
            getTrans (theMap,cur);
        }
        cur = cur->next;
    }
}

void saveRes (LTA::Lta* lta,  Result* res,Options* options)
{
    if (doc!=0)
    {
        map<string,xmlNodePtr> theMap;
        //parse through document to find all the transitions
        getTrans (theMap,xmlDocGetRootElement(doc));

        LTA::LtaIterator* iter = lta->getIterator ();
        ostringstream ostream;
        if (!options->floating)
	  res->outputAlternateSimp (ostream);
        else
            res->outputDoubleSimp (ostream);
        string str = ostream.str ();
        istringstream istr (str);
        if (iter->hasSomething ())
        {
            do
            {
                string buf;
                istr >> buf;
                if (iter->getEdge() != 0)
                    xmlNewProp (theMap[iter->getEdge () ->getId ()],(xmlChar*)"delay",(xmlChar*)buf.c_str ());
            }
            while (iter->move ());
        }
    }
    else
    {
        cout << "Doc is not set" << endl;
    }

    xmlSaveCtxtPtr ctxt = xmlSaveToFilename (options->saveResTo.c_str (),
                          NULL                 ,
                          1         );
    xmlSaveDoc (ctxt,doc);
    xmlSaveClose (ctxt);
    //xmlFree (ctxt);
}


LTA::Lta* createLTA(Options* o)
{
  XMLLoader loader (o->zeroRep);
  LTA::Lta* lta = loader.generateLTA (o->modelpath,&doc,o->clockIndex);
  cout << "LTA created" << endl;
    return lta;
}


void unloadRoutine () {

  if(handle!=0) {
    dlclose (handle);
  }

}

bool loadRoutine (Options* options)
{
  if (options->solver == "entry") {
    solve_plug =solvePoint;
    return true;
  }

  else if (options->solver =="back") {
    solve_plug =  solveSmall;
    return true;
  }
  else if (options->solver =="live") {
    solve_plug = solveLive;
    return true;
    }
  else if (options->solver =="dummy") {
    return false;
  }
  else {
    //no internal matches. We therefore guess it must be an external one...
    string fileToOpen = options->solverPath+"/"+options->solver+".so";

    handle = dlopen (fileToOpen.c_str (),RTLD_LAZY);

    if (handle!=0)
      {
	solve_plug = (solve_func) dlsym (handle,"solve");
	return true;
      }

    else {
      solve_plug =solvePoint;
      return true;
    }


  }
  cerr << "Solver not found" << endl;
  return false;
}

int main(int argc, char** argv)
{

  if (argc == 1) {
    cout << "No arguments specified. Use -h to obtain a list of options" << endl;
    return 1;
  }

  Options* options = loadOptions (argc,argv);
  bool ready = loadRoutine (options);

  LTA::Lta* l = createLTA (options);
  cout << "Trace loaded contains " << l->getNumberOfLocations() << " states." << endl;
  Result* res;
  if (ready && l!= 0)
    {
      res = solve_plug (l,options,cin,cout);
      cout.flush ();
      if (res->hasResult ())
	{

	  if (options->saveRes)
	    {
	      saveRes (l,res,options);
	    }
	  if (!options->quiet){
	    if (!options->floating)
	      {
		if (options->entry)
		  {
		    res->outputAlternateEntry (cout);

		  }
		else
		  {
		    res->outputAlternateDelay (cout);
		  }
	      }
	    else
	      {
		if (options->entry)
		  {
		    res->outputDoubleEntry (cout);
		  }
		else
		  {
		    res->outputDoubleDelay (cout);
		  }
	      }
	    if (options->time)
	      res->outputTiming (cout);
	  }
	}

      delete res;
    }
  if (l!=0)
     delete l;
  delete options;
  if (doc!=0)
    xmlFreeDoc (doc);

  unloadRoutine ();

  return 0;
}

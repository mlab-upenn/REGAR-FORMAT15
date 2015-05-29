

#include "main/Options.hpp"

#include <gmpxx.h>

#include <sstream>
#include <string>


using std::string;
using std::istringstream;
using std::ostringstream;

string readSolveOptions (char** v, int argc, int argN) {
  ostringstream stream;
  while (argN < argc) {
    if (string (v[argN]) != "--") {
      stream << v[argN];
    }
    argN++;
  }
  return stream.str ();
}

string charPtrToString (char* h) {
  istringstream str (h);
  string s;
  str >> s;
  return s;
}

Options* loadOptions (int argc, char** argv) {
  Options* options = new Options;
  int i;
  options->zeroRep = "sys.t(0)";
  options->epsilon = mpz_class ("10");
  options->floating = false;

  options->fast = false;
  options->globalC = "sys.#tau";
  options->quiet = false;
  options->solver = "bigZone";
  while ((i = getopt (argc,argv,"g:s:z:l:e:o:thdfnq"))!=-1){
    switch (i) {
    case 'q':
      options->quiet = true;
      break;
    case 'g':
      options->globalC = charPtrToString (optarg);
      break;

    case 's':
      options->solver = charPtrToString (optarg);
      break;

    case 'l':
      options->solverPath = charPtrToString (optarg);
      break;
    case 'o': //outputfile
      options -> saveResTo = charPtrToString (optarg);
      options -> saveRes  =true;
      break;
    case 't': //measure time
      options -> time = true;
      break;
    case 'f': //output floating points
      options -> floating = true;
      break;
    case 'n': //output entry times
      options -> entry = true;
      break;
    case 'e': //select epsilon
      options -> epsilon = mpz_class (optarg);
      options->fast = true;
      break;
    case 'z':
      options->zeroRep = charPtrToString (optarg);


    }
  }
  
  int argumentNumber = optind;
  if (argumentNumber <argc) {
    istringstream stream (argv[argumentNumber]);
    stream >> options -> modelpath;
    argumentNumber ++;
  }
  
  options->solveOptions = readSolveOptions (argv,argc,argumentNumber);
  
  return options;
}



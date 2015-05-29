

#ifndef _INTERFACE__
#define _INTERFACE__

#include <vector>
#include <string>
#include <iostream>

#include "include/symbTrace/lta.hpp"
#include "include/main/Options.hpp"
using std::vector;
using std::string;
using std::ostream;
using std::istream;


//This class must be implemented by solvers...
class Result {
public:
  virtual bool hasResult () = 0;
  virtual void outputDoubleEntry (ostream&) = 0;
  virtual void outputDoubleDelay (ostream&) = 0;
  virtual void outputAlternateEntry (ostream&) = 0;
  virtual void outputAlternateDelay (ostream&) = 0;
  
  virtual void outputTiming (ostream& ) = 0;
  virtual void outputDoubleSimp (ostream& ) = 0;
  virtual void outputAlternateSimp (ostream& )=0;
};

extern "C" {
  Result* solve (LTA::Lta*, Options*, istream&,  ostream&);
  /*
  void writeOptions (ostream& o);
  
  void writeVersion (ostream& o);
  void writeDescription (ostream& o);
  void doTiming (); */
}

//typedef void (*timing_func)();
typedef Result*  (*solve_func) (LTA::Lta*,Options* , istream&,  ostream&);
/*typedef void (*write_options_func) (ostream& o);
typedef void (*write_version_func) (ostream& o);
typedef void (*write_description_func) (ostream& o);	      
*/
#endif

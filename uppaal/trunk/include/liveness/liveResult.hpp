#ifndef _LIVERESULT__
#define _LIVERESULT__

#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include "solver/interfaceStat.hpp"
#include "solver/interface.hpp"
#include "symbTrace/Location.hpp"
#include "symbTrace/Edge.hpp"
#include "symbTrace/lta.hpp"

using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::istringstream;


namespace liveness {

class LiveResult :public Result  {
private:
  vector<mpq_class> head;
  vector<mpq_class> cycle;
public:
  LiveResult (vector<mpq_class> h,vector<mpq_class> c) {
    head = h;
    cycle = c;
  }
  virtual bool hasResult () {return true;}
  virtual void outputDoubleEntry (ostream& out) {
    out << "Entry times not supported" << endl;
    out << "Outputting delays" << endl;
    outputDoubleDelay (out);
  }
  virtual void outputDoubleDelay (ostream& out) {
    vector<mpq_class>::iterator it;
    for (it = head.begin ();it<head.end ();it++){
      out << "Delay: " << *it << endl;
    }

    out << "The loop" << endl;

    for (it = cycle.begin ();it<cycle.end ();it++){
      out << "Delay: " << *it << endl;
    }
  }
  virtual void outputAlternateEntry (ostream& o) {
    outputDoubleEntry(o);
  }
  virtual void outputAlternateDelay (ostream& o) {
    outputDoubleDelay (o);
  }

  virtual void outputTiming (ostream& o) {}
  virtual void outputDoubleSimp (ostream& o) {}
  virtual void outputAlternateSimp (ostream& o){}

};
}

#endif

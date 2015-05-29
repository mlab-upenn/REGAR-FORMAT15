#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <time.h>
#include <ctime>
#include "solver/interface.hpp"
#include "solver/interfaceStat.hpp"
#include <gmpxx.h>


using std::vector;
using std::cout;
using std::endl;


class DumbResult : public Result{
  private:
    vector<mpq_class> delays;
  bool hasRes;
public:
    DumbResult (vector<mpq_class> d) {
      hasRes = true;
      delays = d;
    }
  
  DumbResult () {hasRes = false;}
    bool hasResult () {return hasRes;}

    void outputDoubleEntry (ostream& out) {
    vector<mpq_class>::iterator it;
    mpq_class entry (0);
    int i = 1;
    for (it = delays.begin ();it!=delays.end ();it++) {
      out << i << " Entry: " << mpf_class (entry) << endl;
      entry += *it;
      i++;
    }
  }

  void outputDoubleDelay (ostream& out) {
    out << "Output floating delay" << endl;
    out.flush ();
    int i = 1;
    vector<mpq_class>::iterator it;
     for (it = delays.begin ();it!=delays.end ();it++) {
       out << i  <<" Delay: " << mpf_class (*it) << endl;
       i++;
     }

  }
  void outputAlternateEntry (ostream& out) {

    vector<mpq_class>::iterator it;
    mpq_class entry (0);
    int i =1;
    for (it = delays.begin ();it!=delays.end ();it++) {
      out << i <<  " Entry: " << entry << endl;
      entry += *it;
      i++;
    }
  }
  void outputAlternateDelay (ostream& out) {

    vector<mpq_class>::iterator it;
    int i = 1;
     for (it = delays.begin ();it!=delays.end ();it++) {
       out << i << " Delay: " << *it << endl;
       i++;
     }
  }

  void outputTiming (ostream& out)  {
    /*out << "Preprocessing " << time.preprocessing <<endl;
      out << "Find Solution " << time.findSolution << endl;*/
  }
  void outputDoubleSimp (ostream& out) {
    vector<mpq_class>::iterator it;
    for (it = delays.begin ();it!=delays.end ();it++) {
      out << mpf_class (*it) << " ";
    }
  }
  void outputAlternateSimp (ostream& out){
    vector<mpq_class>::iterator it;
    for (it = delays.begin ();it!=delays.end ();it++) {
      out << *it << " " ;
    }
  }

  };

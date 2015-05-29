#ifndef _UTIL__
#define _UTIL__

#include <list>
using std::list;

template<class T>
void deinit (list<T*>* t) {
  if (t != 0) {
      //deinit list
       
    typename list<T*>::iterator it;
    for (it=t->begin ();it!=t->end ();it++) {
      delete(*it);
    }
      
    delete t;
  }
};

#endif

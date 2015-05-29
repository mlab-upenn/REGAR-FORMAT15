#ifndef _STORES__
#define _STORES__

#include <iostream>

#include "base/inttypes.h"
#include "system/HashTable.h"

#include "system/Edge.h"
#include "system/Location.h"
#include "system/Process.h"
#include "system/Zone.h"
#include "system/VariableVector.h"
using namespace std;



template<class T>
struct UniqueNode : public HashTable::Node {
  //The instance referred
  T* instance;
  //The id assigned this 
  int id;
};


template<class T> 
class DeinitStore {
private:
  int nextId;
  HashTable table;
  bool deinit;
  void resize (uint32_t val) {

    while(table.getBucket(val) >= table.getSize ()) {
      
      table.resize ();
    }
  }
  
public:
  //No copy constructor as we do not need it
  DeinitStore () : table () {
    nextId = 0;
  }
 
  
  ~DeinitStore () {
    //First we will iterate through the nodes and deallocate them
    //Deallocating the content we however let the use of
    HashTable::Enumerator enume (&table);
    const HashTable::Node* n;
    
    while (enume.getNext (&n)) {
     
      delete (((UniqueNode<T>*)n)->instance);
      delete (n);
    }
    
  }

  int getId ( uint32_t hash) {
    if (exists(hash)) {
      return ((UniqueNode<T>*)(table.find (hash)))->id;
    }
    else 
      return -1;
  }
  
  void insertInstance (T* t, uint32_t hash) {
    resize (hash); //Potentially resize table :-D
    if (!(exists(hash))) {//We will only add entries if they do not already exists
      
      //First create the node
      UniqueNode<T>* node = new UniqueNode<T> ();
      node->instance = t;
      node->id = ++nextId;
      table.insert (node,hash);
      
    }
  }

  T* getInstance (uint32_t hash) {
    return ((UniqueNode<T>*)table.find (hash))->instance;
  }
  
  
  
  bool exists (uint32_t hash) {
    resize (hash); //Potentially resize table :-D
    return (table.find (hash) != NULL);
  }
    
};





template<class T> 
class Store {
private:
  int nextId;
  HashTable table;
  bool deinit;
  void resize (uint32_t val) {

    while(table.getBucket(val) >= table.getSize ()) {
      
      table.resize ();
    }
  }
  
public:
  //No copy constructor as we do not need it
  Store () : table () {
    nextId = 0;
  }
 
  
  ~Store () {
    //First we will iterate through the nodes and deallocate them
    //Deallocating the content we however let the use of
    HashTable::Enumerator enume (&table);
    const HashTable::Node* n;
    
    while (enume.getNext (&n)) {
     
	//delete (((UniqueNode<T>)*n)->instance);
      delete (n);
    }
    
  }

  int getId ( uint32_t hash) {
    if (exists(hash)) {
      return ((UniqueNode<T>*)(table.find (hash)))->id;
    }
    else 
      return -1;
  }
  
  void insertInstance (T* t, uint32_t hash) {
    resize (hash); //Potentially resize table :-D
    if (!(exists(hash))) {//We will only add entries if they do not already exists
      
      //First create the node
      UniqueNode<T>* node = new UniqueNode<T> ();
      node->instance = t;
      node->id = ++nextId;
      table.insert (node,hash);
      
    }
  }
  //I cannot see when this is useful...but perhaps
  T* getInstance (uint32_t hash) {
    return ((UniqueNode<T>*)table.find (hash))->instance;
  }
  
  
  
  bool exists (uint32_t hash) {
    resize (hash); //Potentially resize table :-D
    return (table.find (hash) != NULL);
  }
    
};
 

typedef Store<const Location> LocationStore ;
typedef Store<const Edge> EdgeStore;
typedef Store<const Process> ProcessStore;
typedef Store<const Zone> ZoneStore;
typedef Store<const VariableVector> VariableVectorStore;
#endif

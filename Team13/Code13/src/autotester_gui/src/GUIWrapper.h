#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

#include <PKB.h>
#include <Windows.h>

class GUIWrapper {
 public:
	 PKB* pkb;
  // default constructor
  GUIWrapper();
  
  // destructor
  ~GUIWrapper();
  
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);
  
  // method for evaluating a query
  virtual void Evaluate(std::string query, std::list<std::string>& results);
};

#endif

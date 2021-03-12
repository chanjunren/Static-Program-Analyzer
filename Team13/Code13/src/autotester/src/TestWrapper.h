#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"

class TestWrapper : public AbstractWrapper {
 public:
	 PKB* pkb;
  // default constructor
  TestWrapper();
  
  // destructor
  ~TestWrapper();
  
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);
  
  // method for evaluating a Query
  virtual void Evaluate(std::string Query, std::list<std::string>& results);
};

#endif

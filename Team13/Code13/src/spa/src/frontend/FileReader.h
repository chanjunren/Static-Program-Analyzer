#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#include <vector>

#include "Token.h"

using namespace std;

class FileReader {
public:
	FileReader(string file_name);
	
	string file_name_;
	
	string ReadFile();
};

#endif
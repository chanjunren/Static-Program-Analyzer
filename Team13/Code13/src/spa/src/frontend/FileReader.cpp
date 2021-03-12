#include <fstream>
#include <iostream>

#include "FileReader.h"

using namespace std;

FileReader::FileReader(string file_name) {
	file_name_ = file_name;
}

string FileReader::ReadFile() {

	ifstream ifs(file_name_);
	string input;

	input.assign((istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>()));

	return input;
}
#include "LabelHelper.h"

vector<string> LabelHelper::SplitString(string s) {
	vector<string> words;
	string word;

	for (auto c : s) {
		if (c == ' ') {
			words.push_back(word);
			word = "";
		}
		else {
			word += c;
		}
	}

	words.push_back(word);

	return words;

}
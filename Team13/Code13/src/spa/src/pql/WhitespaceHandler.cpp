#include "WhitespaceHandler.h"

void WhitespaceHandler::TrimLeadingWhitespaces(string* s) {
	int last_leading_space = s->find_first_not_of(" ");
	s->erase(0, last_leading_space);
}

void WhitespaceHandler::TrimTrailingWhitespaces(string* s) {
	int first_trailing_space = s->find_last_not_of(" ") + 1;
	s->erase(first_trailing_space, s->length());
}

void WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(string* s) {
	TrimLeadingWhitespaces(s);
	TrimTrailingWhitespaces(s);
}
#include "PQLCustomTypes.h"

#pragma once
class WhitespaceHandler
{
public:
	static void TrimLeadingWhitespaces(string* s);
	static void TrimTrailingWhitespaces(string* s);
	static void TrimLeadingAndTrailingWhitespaces(string* s);
};


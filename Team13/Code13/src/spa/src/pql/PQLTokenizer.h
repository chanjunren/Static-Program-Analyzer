#pragma once

#include "PQLCustomTypes.h"
#include "WhitespaceHandler.h"
class PQLTokenizer
{
public:
	static STRING_LIST TokenizeBySemicolons(string* query);

	static string RetrieveToken(string* str);

	static string RetrieveTokenByOpenBracket(string* str);

	static string RetrieveTokenByClosingAngleBracket(string* str);

	static string RetrieveTokenByOpeningAngleBracket(string* str);
};


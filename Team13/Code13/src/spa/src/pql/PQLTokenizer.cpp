#include "PQLTokenizer.h"

STRING_LIST PQLTokenizer::TokenizeBySemicolons(string* query) {
    string delimiter = ";";
    size_t pos = 0;
    string token;
    STRING_LIST all_declarations;
    if ((pos = query->find(delimiter)) == string::npos) {
        throw "Error : Invalid format, ';' not found";
    }

    while ((pos = query->find(delimiter)) != string::npos) {
        token = query->substr(0, pos);
        all_declarations.push_back(token);
        query->erase(0, pos + delimiter.length());
    }
    return all_declarations;
}

string PQLTokenizer::RetrieveToken(string* str) {
    WhitespaceHandler::TrimLeadingWhitespaces(str);
    string next_word = str->substr(0, str->find_first_of(" "));
    str->erase(0, str->find_first_of(" "));
    WhitespaceHandler::TrimLeadingWhitespaces(str);
    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&next_word);
    return next_word;
}

string PQLTokenizer::RetrieveTokenByOpenBracket(string* str) {
    WhitespaceHandler::TrimLeadingWhitespaces(str);
    string next_word = str->substr(0, str->find_first_of("("));
    str->erase(0, str->find_first_of("("));
    WhitespaceHandler::TrimLeadingWhitespaces(str);
    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&next_word);
    return next_word;
}

string PQLTokenizer::RetrieveTokenByClosingAngleBracket(string* str) {
    WhitespaceHandler::TrimLeadingWhitespaces(str);
    string next_word = str->substr(0, str->find_first_of(">") + 1);
    str->erase(0, str->find_first_of(">") + 1);
    WhitespaceHandler::TrimLeadingWhitespaces(str);
    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&next_word);
    return next_word;
}

string PQLTokenizer::RetrieveTokenByOpeningAngleBracket(string* str) {
    WhitespaceHandler::TrimLeadingWhitespaces(str);
    string next_word = str->substr(0, str->find_first_of("<"));
    str->erase(0, str->find_first_of("<"));
    WhitespaceHandler::TrimLeadingWhitespaces(str);
    WhitespaceHandler::TrimLeadingAndTrailingWhitespaces(&next_word);
    return next_word;
}
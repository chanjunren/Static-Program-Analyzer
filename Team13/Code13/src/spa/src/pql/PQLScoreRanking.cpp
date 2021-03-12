#include <PQLScoreRanking.h>

PQLScoreRanking::PQLScoreRanking() {
    this->occurrence_count_ = *(new STRING_INTEGER_MAP());
}

PQLScoreRanking::PQLScoreRanking(STRING_INTEGER_MAP occurrence_count) {
    this->occurrence_count_ = occurrence_count;
}

bool PQLScoreRanking::operator()(const STRING_LIST& lhs, const STRING_LIST& rhs) { 
    string l_param1 = ParsingSynonym(lhs[1]);
    string l_param2 = ParsingSynonym(lhs[2]);
    if (lhs.size() == 4) {
        // while & assign pattern
        l_param2 = ParsingSynonym(lhs[3]);;
    }
    else if (lhs.size() == 5) {
        // if pattern
        l_param2 = ParsingSynonym(lhs[4]);;
    }

    string r_param1 = ParsingSynonym(rhs[1]);
    string r_param2 = ParsingSynonym(rhs[2]);
    if (rhs.size() == 4) {
        // while & assign pattern
        r_param2 = ParsingSynonym(rhs[3]);;
    }
    else if (rhs.size() == 5) {
        // if pattern
        r_param2 = ParsingSynonym(rhs[4]);;
    }

    int lhs_priority = GetTotalScore(GetOccurrenceScore(l_param1), GetOccurrenceScore(l_param2));
    int rhs_priority = GetTotalScore(GetOccurrenceScore(r_param1), GetOccurrenceScore(r_param2));
    
    lhs_priority *= ranks_of_functions.at(lhs[0]);
    rhs_priority *= ranks_of_functions.at(rhs[0]);

    // Ranking: 1-synonym clauses -> 2-synonyms clauses, rank by function & synonym occurrence
    return  lhs_priority > rhs_priority;
}

int PQLScoreRanking::GetOccurrenceScore(string target) {
    int score = 0;

    if (!target.empty() && occurrence_count_.find(target) != occurrence_count_.end()) {
        score = occurrence_count_.at(target);
    }

    return score;
}

int PQLScoreRanking::GetTotalScore(int param1_score, int param2_score) {
    int total_score = param1_score + param2_score;

    return param1_score > 0 && param2_score > 0 ? total_score * 1000 : total_score;
}

string PQLScoreRanking::ParsingSynonym(string synonym) {
    string parsed_synonym = "";

    // remove attribute if present
    int index = synonym.find(".");
    index != string::npos ? parsed_synonym = synonym.substr(0, index) : parsed_synonym = synonym;

    // check for str/int
    return parsed_synonym[0] == '\"' && parsed_synonym[parsed_synonym.length() - 1] == '\"' 
        ? "" : find_if(parsed_synonym.begin(), parsed_synonym.end(), [](unsigned char c) { return !isdigit(c); }) == parsed_synonym.end() 
        ? "" : parsed_synonym;
}
#pragma once

#include <PQLCustomTypes.h>

class PQLScoreRanking {
private:
    STRING_INTEGER_MAP occurrence_count_;
    STRING_INTEGER_MAP ranks_of_functions = {
        { TYPE_WITH_CLAUSE,     1 },
        { TYPE_COND_FOLLOWS,    20 },
        { TYPE_COND_MODIFIES_S, 30 },
        { TYPE_COND_MODIFIES_P, 40 },
        { TYPE_COND_PARENT,     50 },
        { TYPE_COND_USES_S,     60 },
        { TYPE_COND_USES_P,     70 },
        { TYPE_COND_CALLS,      80 },
        { TYPE_COND_FOLLOWS_T,  90 },
        { TYPE_COND_PARENT_T,   100 },
        { TYPE_COND_CALLS_T,    110 },
        { TYPE_COND_NEXT,       120 },
        { TYPE_COND_PATTERN_F,  130 },
        { TYPE_COND_PATTERN_P,  140 },
        { TYPE_COND_NEXT_T,     500 },
        { TYPE_COND_AFFECTS,    600 },
        { TYPE_COND_AFFECTS_T,  700 }
    };

public:
    PQLScoreRanking();
    PQLScoreRanking(STRING_INTEGER_MAP occurrence_count);

    int GetOccurrenceScore(string target);
    int GetTotalScore(int param1_score, int param2_score);
    string ParsingSynonym(string synonym);

    bool operator()(const STRING_LIST& lhs, const STRING_LIST& rhs);
};
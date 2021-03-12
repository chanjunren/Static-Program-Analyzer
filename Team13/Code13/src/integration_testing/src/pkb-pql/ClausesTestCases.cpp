#include "catch.hpp"
using namespace std;

#include "pkb/PKB.h"
#include "pql/PQLEvaluator.h"

PKB pkb_st = PKB();
PQLEvaluator pql_st = PQLEvaluator();

void IsSameResult(STRINGLIST_SET pkb_two_result, STRINGLIST_SET pql_two_result) {
	bool is_found = false;

	for (STRING_LIST* entry1 : pkb_two_result) {
		for (STRING_LIST* entry2 : pql_two_result) {
			if (*entry1 == *entry2) {
				is_found = true;
			}
		}
	}

	REQUIRE(is_found == true);
}

TEST_CASE("Follows") {
	pkb_st.GetRelationManager().AddFollows(1, 2);
	pkb_st.GetRelationManager().AddFollows(3, 4);

	pkb_st.GetRelationManager().AddFollows(7, 8);
	pkb_st.GetRelationManager().AddFollows(8, 9);
	pkb_st.GetRelationManager().AddFollows(9, 10);
	
	// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsFollows(1, 2) == pql_st.EvaluateNoSynonymSet(TYPE_COND_FOLLOWS, "1", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsFollows(-1, 2) == pql_st.EvaluateNoSynonymSet(TYPE_COND_FOLLOWS, "_", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsFollows(1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_FOLLOWS, "1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsFollows(1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_FOLLOWS, "_", "_"));

	REQUIRE(pkb_st.GetRelationManager().IsFollows(-1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_FOLLOWS, "", ""));
	REQUIRE(pkb_st.GetRelationManager().IsFollows(-1, 2) == pql_st.EvaluateNoSynonymSet(TYPE_COND_FOLLOWS, "", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsFollows(1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_FOLLOWS, "1", ""));

	// ONE user-declared entities
	STRINGLIST_SET pkb_one_result = STRINGLIST_SET();
	STRINGLIST_SET pql_one_result = STRINGLIST_SET();

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetFollows(1));
	pql_one_result = pql_st.EvaluateOneSynonymSet(TYPE_COND_FOLLOWS, "1");
	IsSameResult(pkb_one_result , pql_one_result);
	
	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetFollows(-1));
	pql_one_result = pql_st.EvaluateOneSynonymSet(TYPE_COND_FOLLOWS, "_");
	IsSameResult(pkb_one_result , pql_one_result);
	
	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseFollows(1));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_FOLLOWS, "1");
	IsSameResult(pkb_one_result , pql_one_result);
	
	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseFollows(-1));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_FOLLOWS, "_");
	IsSameResult(pkb_one_result , pql_one_result);

	// TWO user-declared entities
	STRINGLIST_SET pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllFollows());
	STRINGLIST_SET pql_two_result = pql_st.EvaluateTwoSynonymSet(TYPE_COND_FOLLOWS);
	IsSameResult(pkb_two_result, pql_two_result);
}

TEST_CASE("FollowsT") {
	pkb_st.GetRelationManager().AddFollowsStar(1, 2);
	pkb_st.GetRelationManager().AddFollowsStar(3, 4);

	pkb_st.GetRelationManager().AddFollowsStar(7, 8);
	pkb_st.GetRelationManager().AddFollowsStar(8, 9);
	pkb_st.GetRelationManager().AddFollowsStar(9, 10);

	pkb_st.GetRelationManager().AddFollowsStar(7, 9);
	pkb_st.GetRelationManager().AddFollowsStar(7, 10);
	pkb_st.GetRelationManager().AddFollowsStar(8, 10);

	// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsFollowsStar(1, 2) == pql_st.EvaluateNoSynonymSet(TYPE_COND_FOLLOWS_T, "1", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsFollowsStar(-1, 2) == pql_st.EvaluateNoSynonymSet(TYPE_COND_FOLLOWS_T, "_", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsFollowsStar(1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_FOLLOWS_T, "1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsFollowsStar(1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_FOLLOWS_T, "_", "_"));

	REQUIRE(pkb_st.GetRelationManager().IsFollowsStar(-1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_FOLLOWS_T, "", ""));
	REQUIRE(pkb_st.GetRelationManager().IsFollowsStar(-1, 2) == pql_st.EvaluateNoSynonymSet(TYPE_COND_FOLLOWS_T, "", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsFollowsStar(1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_FOLLOWS_T, "1", ""));

	// ONE user-declared entities
	STRINGLIST_SET pkb_one_result = STRINGLIST_SET();
	STRINGLIST_SET pql_one_result = STRINGLIST_SET();

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetFollowsStars(1));
	pql_one_result = pql_st.EvaluateOneSynonymSet(TYPE_COND_FOLLOWS_T, "1");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetFollowsStars(-1));
	pql_one_result = pql_st.EvaluateOneSynonymSet(TYPE_COND_FOLLOWS_T, "_");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseFollowsStars(1));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_FOLLOWS_T, "1");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseFollowsStars(-1));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_FOLLOWS_T, "_");
	IsSameResult(pkb_one_result , pql_one_result);

	// TWO user-declared entities
	STRINGLIST_SET pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllFollowsStars());
	STRINGLIST_SET pql_two_result = pql_st.EvaluateTwoSynonymSet(TYPE_COND_FOLLOWS_T);
	IsSameResult(pkb_two_result, pql_two_result);
}

TEST_CASE("Parent") {
	pkb_st.GetRelationManager().AddParent(1, 2);
	pkb_st.GetRelationManager().AddParent(3, 4);

	pkb_st.GetRelationManager().AddParent(7, 8);
	pkb_st.GetRelationManager().AddParent(8, 9);
	pkb_st.GetRelationManager().AddParent(9, 10);

	pkb_st.GetRelationManager().AddParent(7, 9);
	pkb_st.GetRelationManager().AddParent(7, 10);
	pkb_st.GetRelationManager().AddParent(8, 10);

	// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsParent(1, 2) == pql_st.EvaluateNoSynonymSet(TYPE_COND_PARENT, "1", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsParent(-1, 2) == pql_st.EvaluateNoSynonymSet(TYPE_COND_PARENT, "_", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsParent(1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_PARENT, "1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsParent(1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_PARENT, "_", "_"));

	REQUIRE(pkb_st.GetRelationManager().IsParent(-1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_PARENT, "", ""));
	REQUIRE(pkb_st.GetRelationManager().IsParent(-1, 2) == pql_st.EvaluateNoSynonymSet(TYPE_COND_PARENT, "", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsParent(1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_PARENT, "1", ""));

	// ONE user-declared entities
	STRINGLIST_SET pkb_one_result = STRINGLIST_SET();
	STRINGLIST_SET pql_one_result = STRINGLIST_SET();

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetParents(1));
	pql_one_result = pql_st.EvaluateOneSynonymSet(TYPE_COND_PARENT, "1");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetParents(-1));
	pql_one_result = pql_st.EvaluateOneSynonymSet(TYPE_COND_PARENT, "_");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseParents(1));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_PARENT, "1");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseParents(-1));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_PARENT, "_");
	IsSameResult(pkb_one_result , pql_one_result);

	// TWO user-declared entities
	STRINGLIST_SET pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllParents());
	STRINGLIST_SET pql_two_result = pql_st.EvaluateTwoSynonymSet(TYPE_COND_PARENT);
	IsSameResult(pkb_two_result, pql_two_result);

}

TEST_CASE("ParentT") {
	pkb_st.GetRelationManager().AddParentStar(1, 2);
	pkb_st.GetRelationManager().AddParentStar(3, 4);

	pkb_st.GetRelationManager().AddParentStar(7, 8);
	pkb_st.GetRelationManager().AddParentStar(8, 9);
	pkb_st.GetRelationManager().AddParentStar(9, 10);

	pkb_st.GetRelationManager().AddParentStar(7, 9);
	pkb_st.GetRelationManager().AddParentStar(7, 10);
	pkb_st.GetRelationManager().AddParentStar(8, 10);

	// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsParentStar(1, 2) == pql_st.EvaluateNoSynonymSet(TYPE_COND_PARENT_T, "1", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsParentStar(-1, 2) == pql_st.EvaluateNoSynonymSet(TYPE_COND_PARENT_T, "_", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsParentStar(1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_PARENT_T, "1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsParentStar(1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_PARENT_T, "_", "_"));

	REQUIRE(pkb_st.GetRelationManager().IsParentStar(-1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_PARENT_T, "", ""));
	REQUIRE(pkb_st.GetRelationManager().IsParentStar(-1, 2) == pql_st.EvaluateNoSynonymSet(TYPE_COND_PARENT_T, "", "2"));
	REQUIRE(pkb_st.GetRelationManager().IsParentStar(1, -1) == pql_st.EvaluateNoSynonymSet(TYPE_COND_PARENT_T, "1", ""));

	// ONE user-declared entities
	STRINGLIST_SET pkb_one_result = STRINGLIST_SET();
	STRINGLIST_SET pql_one_result = STRINGLIST_SET();

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetParentStars(1));
	pql_one_result = pql_st.EvaluateOneSynonymSet(TYPE_COND_PARENT_T, "1");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetParentStars(-1));
	pql_one_result = pql_st.EvaluateOneSynonymSet(TYPE_COND_PARENT_T, "_");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseParentStars(1));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_PARENT_T, "1");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseParentStars(-1));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_PARENT_T, "_");
	IsSameResult(pkb_one_result , pql_one_result);

	// TWO user-declared entities
	STRINGLIST_SET pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllParentStars());
	STRINGLIST_SET pql_two_result = pql_st.EvaluateTwoSynonymSet(TYPE_COND_PARENT_T);
	IsSameResult(pkb_two_result, pql_two_result);

}

TEST_CASE("UsesS") {
	pkb_st.GetRelationManager().AddStmtUses(1, "v");
	pkb_st.GetRelationManager().AddStmtUses(3, "v");
	pkb_st.GetRelationManager().AddStmtUses(7, "v");

	//// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsStmtUses(1, "v") == pql_st.EvaluateNoSynonymSet(TYPE_COND_USES_S, "1", "v"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtUses(1, "") == pql_st.EvaluateNoSynonymSet(TYPE_COND_USES_S, "1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtUses(1, "v") == pql_st.EvaluateNoSynonymSet(TYPE_COND_USES_S, "1", ""));
	REQUIRE(pkb_st.GetRelationManager().IsStmtUses(-1, "") == pql_st.EvaluateNoSynonymSet(TYPE_COND_USES_S, "", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtUses(-1, "v") == pql_st.EvaluateNoSynonymSet(TYPE_COND_USES_S, "", "v"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtUses(-1, "") == pql_st.EvaluateNoSynonymSet(TYPE_COND_USES_S, "", ""));

	STRINGLIST_SET pkb_one_result = STRINGLIST_SET();
	STRINGLIST_SET pql_one_result = STRINGLIST_SET();

	//// ONE user-declared entities
	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetStmtUses(1));
	pql_one_result = pql_st.EvaluateOneSynonymSet(TYPE_COND_USES_S, "1");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseStmtUses("v"));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_USES_S, "v");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseStmtUses(""));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_USES_S, "_");
	IsSameResult(pkb_one_result , pql_one_result);

	//// TWO user-declared entities
	STRINGLIST_SET pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllStmtUses());
	STRINGLIST_SET pql_two_result = pql_st.EvaluateTwoSynonymSet(TYPE_COND_USES_S);
	IsSameResult(pkb_two_result, pql_two_result);
}

TEST_CASE("UsesP") {
	pkb_st.GetRelationManager().AddProcUses("v1", "v2");
	pkb_st.GetRelationManager().AddProcUses("v1", "v2");
	pkb_st.GetRelationManager().AddProcUses("v1", "v2");

	//// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsProcUses("v1", "v2") == pql_st.EvaluateNoSynonymSet(TYPE_COND_USES_P, "v1", "v2"));
	REQUIRE(pkb_st.GetRelationManager().IsProcUses("v1", "") == pql_st.EvaluateNoSynonymSet(TYPE_COND_USES_P, "v1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsProcUses("v1", "") == pql_st.EvaluateNoSynonymSet(TYPE_COND_USES_P, "v1", ""));
	REQUIRE(pkb_st.GetRelationManager().IsProcUses("", "") == pql_st.EvaluateNoSynonymSet(TYPE_COND_USES_P, "", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsProcUses("", "v2") == pql_st.EvaluateNoSynonymSet(TYPE_COND_USES_P, "", "v2"));
	REQUIRE(pkb_st.GetRelationManager().IsProcUses("", "") == pql_st.EvaluateNoSynonymSet(TYPE_COND_USES_P, "", ""));

	STRINGLIST_SET pkb_one_result = STRINGLIST_SET();
	STRINGLIST_SET pql_one_result = STRINGLIST_SET();

	//// ONE user-declared entities
	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetProcUses("v1"));
	pql_one_result = pql_st.EvaluateOneSynonymSet(TYPE_COND_USES_P, "v1");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseProcUses("v2"));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_USES_P, "v2");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseProcUses(""));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_USES_P, "_");
	IsSameResult(pkb_one_result , pql_one_result);

	//// TWO user-declared entities
	STRINGLIST_SET pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllProcUses());
	STRINGLIST_SET pql_two_result = pql_st.EvaluateTwoSynonymSet(TYPE_COND_USES_P);
	IsSameResult(pkb_two_result, pql_two_result);
}

TEST_CASE("ModifiesS") {
	pkb_st.GetRelationManager().AddStmtModifies(1, "v");
	pkb_st.GetRelationManager().AddStmtModifies(3, "v");
	pkb_st.GetRelationManager().AddStmtModifies(7, "v");

	//// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsStmtModifies(1, "v") == pql_st.EvaluateNoSynonymSet(TYPE_COND_MODIFIES_S, "1", "v"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtModifies(1, "") == pql_st.EvaluateNoSynonymSet(TYPE_COND_MODIFIES_S, "1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtModifies(1, "v") == pql_st.EvaluateNoSynonymSet(TYPE_COND_MODIFIES_S, "1", ""));
	REQUIRE(pkb_st.GetRelationManager().IsStmtModifies(-1, "") == pql_st.EvaluateNoSynonymSet(TYPE_COND_MODIFIES_S, "", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtModifies(-1, "v") == pql_st.EvaluateNoSynonymSet(TYPE_COND_MODIFIES_S, "", "v"));
	REQUIRE(pkb_st.GetRelationManager().IsStmtModifies(-1, "") == pql_st.EvaluateNoSynonymSet(TYPE_COND_MODIFIES_S, "", ""));

	STRINGLIST_SET pkb_one_result = STRINGLIST_SET();
	STRINGLIST_SET pql_one_result = STRINGLIST_SET();

	//// ONE user-declared entities
	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetStmtModifies(1));
	pql_one_result = pql_st.EvaluateOneSynonymSet(TYPE_COND_MODIFIES_S, "1");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseStmtModifies("v"));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_MODIFIES_S, "v");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseStmtModifies(""));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_MODIFIES_S, "_");
	IsSameResult(pkb_one_result , pql_one_result);

	//// TWO user-declared entities
	STRINGLIST_SET pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllStmtModifies());
	STRINGLIST_SET pql_two_result = pql_st.EvaluateTwoSynonymSet(TYPE_COND_MODIFIES_S);
	IsSameResult(pkb_two_result, pql_two_result);
}

TEST_CASE("ModifiesP") {
	pkb_st.GetRelationManager().AddProcModifies("v1", "v2");
	pkb_st.GetRelationManager().AddProcModifies("v1", "v2");
	pkb_st.GetRelationManager().AddProcModifies("v1", "v2");

	//// NO user-declared entities
	REQUIRE(pkb_st.GetRelationManager().IsProcModifies("v1", "v2") == pql_st.EvaluateNoSynonymSet(TYPE_COND_MODIFIES_P, "v1", "v2"));
	REQUIRE(pkb_st.GetRelationManager().IsProcModifies("v1", "") == pql_st.EvaluateNoSynonymSet(TYPE_COND_MODIFIES_P, "v1", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsProcModifies("v1", "") == pql_st.EvaluateNoSynonymSet(TYPE_COND_MODIFIES_P, "v1", ""));
	REQUIRE(pkb_st.GetRelationManager().IsProcModifies("", "") == pql_st.EvaluateNoSynonymSet(TYPE_COND_MODIFIES_P, "", "_"));
	REQUIRE(pkb_st.GetRelationManager().IsProcModifies("", "v2") == pql_st.EvaluateNoSynonymSet(TYPE_COND_MODIFIES_P, "", "v2"));
	REQUIRE(pkb_st.GetRelationManager().IsProcModifies("", "") == pql_st.EvaluateNoSynonymSet(TYPE_COND_MODIFIES_P, "", ""));

	STRINGLIST_SET pkb_one_result = STRINGLIST_SET();
	STRINGLIST_SET pql_one_result = STRINGLIST_SET();

	//// ONE user-declared entities
	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetProcModifies("v1"));
	pql_one_result = pql_st.EvaluateOneSynonymSet(TYPE_COND_MODIFIES_P, "v1");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseProcModifies("v2"));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_MODIFIES_P, "v2");
	IsSameResult(pkb_one_result , pql_one_result);

	pkb_one_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetInverseProcModifies(""));
	pql_one_result = pql_st.EvaluateInverseOneSynonymSet(TYPE_COND_MODIFIES_P, "_");
	IsSameResult(pkb_one_result , pql_one_result);

	//// TWO user-declared entities
	STRINGLIST_SET pkb_two_result = pql_st.ConvertSet(pkb_st.GetRelationManager().GetAllProcModifies());
	STRINGLIST_SET pql_two_result = pql_st.EvaluateTwoSynonymSet(TYPE_COND_MODIFIES_P);
	IsSameResult(pkb_two_result, pql_two_result);
}
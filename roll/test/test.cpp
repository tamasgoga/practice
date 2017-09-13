#include "basic.hpp"

#include <string>
#include <iostream>

using namespace std;


static inline void printTestSeparator(const string& title = "", bool isfirstTest = false) {
	printf("%s------ %s ------\n", !isfirstTest ? "\n\n" : "", title.c_str());
}


void runAllTests() {
	printTestSeparator("ROLL 1 <Die>", true);
	testDieRoll(100, 5000);

	printTestSeparator("COPY & ASIGN <Die>");
	testCopyAssignDie(1, 1);

	printTestSeparator("ROLL 1 <Roll> FEW TIMES");
	testRollObj({"3d20", "6d6,  2d12 6d4  d6 "}, 2);

	printTestSeparator("TEST ERRORS");
	testRollObjCtorError();

	puts("\n\nDone.\nAll tests succeeded!");
}
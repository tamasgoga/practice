#include "basic.hpp"

#include <string>
#include <iostream>

using namespace std;


static inline void printTestSeparator(const string& title = "", bool isfirstTest = false) {
	printf("%s------ %s ------\n", !isfirstTest ? "\n\n" : "", title.c_str());
}


void runAllTests() {
	printTestSeparator("ROLL 1 <Die>", true);
	testDieRoll();

	printTestSeparator("COPY & ASIGN <Die>");
	testCopyAssignDie();

	printTestSeparator("ROLL 1 <Roll> FEW TIMES");
	testRollObj();

	printTestSeparator("TEST ERRORS");
	testRollObjCtorError();

	puts("\n\nDone.\nAll tests succeeded!");
}

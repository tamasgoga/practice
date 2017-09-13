#include "../src/Roll.hpp"
#include "../src/io.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <exception>

using namespace std;


void testDieRoll() {
	constexpr Die::value_type type = 100;
	constexpr int numOfRolls = 10000;

	int faultyRollCount = 0;

	Die d(type);
	for (int i = 0; i < numOfRolls; ++i) {
		if (d.get() < 1 || d.get() > type)
			++faultyRollCount;

		d.roll();
	}

	if (faultyRollCount == 0)
		cout <<  "All " <<  numOfRolls << " die rolls generated correctly!" << endl;
	else
		cout << "Faulty die rolls: " << faultyRollCount << " out of " << numOfRolls << endl;

	assert(faultyRollCount == 0);
}


void testCopyAssignDie() {
	constexpr Die::value_type type = 1;
	constexpr Die::value_type val = 1;

	cout << "Copy: ";
	Die d1(type, val);
	Die d2 = d1;
	Die d3(d1);
	cout << "\n\td1 (" << &d1 << ") " << d1 << "\n\td2 (" << &d2 << ") " << d2 << "\n\td3 (" << &d3 << ") " << d3 << endl;

	assert((&d1 != &d2) && (&d1 != &d2) && (d1 == d2) && (d1 == d3));

	cout << "\nAssign: ";
	d1 = val;
	d2 = d1;
	cout << "\n\td1 (" << &d1 << ") " << d1 << "\n\td2 (" << &d2 << ") " << d2 << endl;

	assert((&d1 != &d2) && (d1 == d2));
}


void testRollObj() {
	// setup
	const std::vector<std::string> command = {"3d20", "6d6,  2d12 6d4  d6 "};

	// test
	Roll roll(command);
	printRoll(roll, "roll #1");

	roll.roll();
	putchar('\n');
	printRoll(roll, "roll #2");
}


void testRollObjCtorError() {
	// (!) expand to test all cases

	try {
		cout << "> calling Roll({\"3d20\", \"6de6,  2d12\"})" << endl;
		Roll({"3d20", "6de6,  2d12"});
	} catch (const invalid_argument& e) {
		cout << "  SUCCESS! Got invalid_argument: " << e.what() << endl;
	} catch (...) {
		throw;
	}
}

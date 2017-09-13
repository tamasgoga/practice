/* Dice roll simulator by Goga Tamas, 2017 */

#include "src/Roll.hpp"
#include "src/io.hpp"

#ifdef ROLL_TESTING
#include "test/test.hpp"
#endif // ROLL_TESTING

#include <iostream>
#include <stdexcept>

using namespace std;


//---------------------------------------------------
// Main functionality
//---------------------------------------------------


int main(int argc, char** argv) {
	try {
		if (argc == 1) {
			cout << "Roll any number of dice (by Goga Tamas)\n  e.g. \""
			     << argv[0] << " 3d20\"\n       \""
			     << argv[0] << " 6d6 2d12 6d4 d6\""
#ifdef ROLL_TESTING
			        "\n       \"" << argv[0] << " test\""
#endif // ROLL_TESTING
			        "\n\n  syntax: <multiplier>d<die_type>, no negative numbers" << endl;
			return 0;
		}

		vector<string> cmd;
		cmd.reserve(argc - 1);

		for (int i = 1; i < argc; ++i)
			cmd.emplace_back(argv[i]);

#ifdef ROLL_TESTING
		if (cmd.size() == 1 && cmd[0] == "test") {
			runAllTests();
			return 0;
		}
#endif // ROLL_TESTING

		Roll r(cmd);
		printRoll(r, "Roll");
	} catch (const invalid_argument& e) {
		cerr << "BAD ARGUMENT: " << e.what() << endl;
	} catch (const out_of_range& e) {
		cerr << "RANGE ERROR: " << e.what() << endl;
	} catch (const bad_alloc& e) {
		cerr << "ALLOC ERROR: Unable to allocate enough memory" << endl;
	} catch (const exception& e) {
		cerr << "ERROR: " << e.what() << endl;
	}
}
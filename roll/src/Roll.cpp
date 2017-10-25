#include "Roll.hpp"

#include <sstream>
#include <stdexcept>

using namespace std;


//---------------------------------------------------
// Utility
//---------------------------------------------------


/* Split a string based on a delimiting char */
static vector<string> split(const string& str, char delim) {
	vector<string> res;
	stringstream ss(str);

	string item;
	while (getline(ss, item, delim)) {
		res.push_back(item);
	}

	return res;
}


/* Converts a number with proper exception messages */
static Die::value_type strToValType(const string& str,
                                    const string& title = "Invalid argument",
                                    const string& context = "unspecified")
{
	try {
		return static_cast<Die::value_type>(stoul(str));
	} catch (const invalid_argument& e) {
		throw invalid_argument(title + ": \"" + str + "\" in \"" + context + "\"");
	} catch (const out_of_range& e) {
		throw out_of_range(string("Number too large: \"") + str + "\" in \"" + context + "\"");
	}
}


//---------------------------------------------------
// class: Roll
//---------------------------------------------------


/* e.g. cmd = {"3d6", "3d6 2d12"} */
Roll::Roll(const vector<string>& commands)
	: dice()
{
	size_type diceNum = commands.size();
	dice.reserve(diceNum);

	for (const auto& cmd: commands) {
		auto subCmd = split(cmd, ' ');

		if (subCmd.size() > 1) {
			diceNum += subCmd.size() - 1;
			dice.reserve(diceNum);
		}

		for (const auto& oneCmd: subCmd) {
			if (oneCmd.empty())
				continue;

			auto diceStr = split(oneCmd, 'd');
			if (diceStr.size() != 2)
				throw invalid_argument(string("Invalid Roll: \"") + oneCmd + "\" in \"" + cmd + "\"");

			Die::value_type multiplier = 1;
			if (!diceStr[0].empty()) {
				auto firstNonSpace = diceStr[0].find_first_not_of(" \f\n\r\t\v");
				if (firstNonSpace != string::npos && diceStr[0][firstNonSpace] ==  '-')
					throw invalid_argument(string("Negative die multiplier: \"") + diceStr[0] + "\" in \"" + cmd + "\"");

				multiplier = strToValType(diceStr[0], "Invalid die multiplier", oneCmd);
			}

			auto firstNonSpace = diceStr[1].find_first_not_of(" \f\n\r\t\v");
			if (firstNonSpace != string::npos && diceStr[1][firstNonSpace] ==  '-')
				throw invalid_argument(string("Negative die type: \"") + diceStr[0] + "\" in \"" + cmd + "\"");

			Die::value_type dieType = strToValType(diceStr[1], "Invalid die type", oneCmd);

			if (multiplier < 1)
				throw invalid_argument(string("Invalid die multiplier: \"") + diceStr[0] + "\" in \"" + cmd + "\"");

			diceNum += multiplier - 1;
			dice.reserve(diceNum);

			for (Die::value_type i = 0; i < multiplier; ++i) {
				dice.emplace_back(dieType);
			}
		}
	}
}


Roll::Roll(const Roll& other)
	: dice(other.dice)
{}


/* Print the average and total, used by printRoll() */
static inline void printInfo(ostream& out, Die::value_type total, Die::value_type count) {
	double avg = count > 0 ? static_cast<double>(total) / count : 0.0;
	out << "\n    (total: " << total << ", average: " << avg << ')';
}


ostream& operator<<(ostream& out, const Roll& r) {
	if (r.dice.size() == 0) {
		out << "Roll: empty...";
		return out;
	}

	static constexpr auto ZERO = static_cast<Die::value_type>(0);
	auto dieType = r.dice[0].type;
	auto total = ZERO;
	auto count = ZERO;

	out << "d" << dieType << " -> ";

	for (const auto& d: r.dice) {
		if (d.type != dieType) {
			dieType = d.type;
			printInfo(out, total, count);
			out << "\nd" << dieType << " -> ";
			total = count = ZERO;
		}

		++count;
		total += d.get();

		out << d.get() << ' ';
	}

	if (count > ZERO)
		printInfo(out, total, count);

#ifdef ROLL_TESTING
	out << "\n\nDice count: " << r.dice.size() << "\nDice storage size: " << r.dice.capacity();
#endif // ROLL_TESTING

	return out;
}

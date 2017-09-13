#include "io.hpp"
#include "Roll.hpp"

#include <iostream>

using namespace std;


/* Print the average and total, used by printRoll() */
static inline void printInfo(Die::value_type total, Die::value_type count) {
	double avg = count > 0 ? static_cast<double>(total) / count : 0.0;
	cout << "\n    (total: " << total << ", average: " << avg << ')';
}


void printRoll(const Roll& r, const string& title) {
	if (r.dice.size() == 0) {
		cout << title << ": empty..." << endl;
		return;
	}

	static constexpr auto ZERO = static_cast<Die::value_type>(0);
	auto dieType = r.dice[0].type;
	auto total = ZERO;
	auto count = ZERO;

	cout << title << ": " << '\n';
	cout << "  d" << dieType << " -> ";

	for (const auto& d: r.dice) {
		if (d.type != dieType) {
			dieType = d.type;
			printInfo(total, count);
			cout << "\n  d" << dieType << " -> ";
			total = count = ZERO;
		}

		++count;
		total += d.get();

		cout << d.get() << ' ';
	}

	if (count > ZERO)
		printInfo(total, count);

#ifdef ROLL_TESTING
	cout << "\nDice count: " << r.dice.size() << "\nDice storage size: " << r.dice.capacity();
#endif // ROLL_TESTING

	cout << endl;
}
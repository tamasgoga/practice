// examples & tests for the fractions library
#include "gg_fractions.hpp"

#include <iostream>
#include <vector>
#include <ctime>

using namespace std;


template <typename T>
void testFractions(T n1, T d1, T n2, T d2) {
	#define tab "    "
	#define couttab cout << tab
	#define endltab '\n' << tab
	#define dblendl "\n\n"

	gg::fraction<T> f1(n1, d1);
	gg::fraction<T> f2(n2, d2);

	try {
		cout << "Testing operations on fractions " << f1 << " and " << f2 << ";\n"
		     << "Operation = result = reduced result:\n";

		auto inv1 = f1.reciprocal();
		auto inv2 = f2.reciprocal();
		auto neg1 = -f1;
		auto neg2 = -f2;

		auto add = f1 + f2;
		auto sub = f1 - f2;
		auto mul = f1 * f2;
		auto div = f1 / f2;

		couttab << "inv(" << f1 << ") = " << inv1 << " = " << inv1.humanize() << endltab
		        << "inv(" << f2 << ") = " << inv2 << " = " << inv2.humanize() << endltab
		        << "-(" << f1 << ") = " << neg1 << " = " << neg1.humanize() << endltab
		        << "-(" << f2 << ") = " << neg2 << " = " << neg2.humanize() << dblendl << tab
		        << f1 << " + " << f2 << " = " << add << " = " << add.humanize() << endltab
		        << f1 << " - " << f2 << " = " << sub << " = " << sub.humanize() << endltab
		        << f1 << " * " << f2 << " = " << mul << " = " << mul.humanize() << endltab
		        << f1 << " / " << f2 << " = " << div << " = " << div.humanize() << dblendl;

		couttab << f1 << " += " << f2 << " = ";
		f1 += f2;
		cout << f1 << " = " << f1.humanize() << '\n';

		couttab << f1 << " -= " << f2 << " = ";
		f1 -= f2;
		cout << f1 << " = " << f1.humanize() << '\n';

		couttab << f1 << " *= " << f2 << " = ";
		f1 *= f2;
		cout << f1 << " = " << f1.humanize() << '\n';

		couttab << f1 << " /= " << f2 << " = ";
		f1 /= f2;
		cout << f1 << " = " << f1.humanize() << dblendl;

		f1 = gg::fraction<T>(n1, d1);
		couttab << "(long double) " << f1 << " = " << (long double)(f1) << endltab
		        << "(double)      " << f1 << " = " << double(f1)        << endltab
		        << "(float)       " << f1 << " = " << float(f1)         << endltab
		        << "(type)        " << f1 << " = " << f1.toType()       << endltab
		        << "(long double) " << f2 << " = " << (long double)(f2) << endltab
		        << "(double)      " << f2 << " = " << double(f2)        << endltab
		        << "(float)       " << f2 << " = " << float(f2)         << endltab
		        << "(type)        " << f2 << " = " << f2.toType()       << dblendl;

		auto fof = gg::fraction<T>(f1, f2);
		couttab << "Fraction of fractions: " << fof << " = " << fof.humanize() << '\n';

		puts("Done!\n");
	} catch (const gg::IllegalFraction& ife) {
		printf(tab "Exception: %s\nTerminated.\n\n", ife.what());
	} catch (const exception& e) {
		printf(tab "(!) ERROR: %s\nTerminated.\n\n", e.what());
	}

	fflush(stdin);

	#undef tab
	#undef couttab
	#undef endltab
	#undef dblendl
}


void arithOpTest() {
	puts("--- (int) ---");
	testFractions<int>(10,4, 2,4);

	puts("--- (float) ---");
	testFractions<float>(3,3, 9,3);

	/* this will fail: */
	puts("--- (int: div by 0) ---");
	testFractions<int>(10,0, 2,4);
}


void compOpTest() {
	using rational = gg::fraction<int>;
	using rvec = vector<rational>;

	rvec nums;
	nums.emplace_back( 3,3);
	nums.emplace_back(-3,9);
	nums.emplace_back( 4,4);
	nums.emplace_back( 0,0);
	nums.emplace_back( 1,3);
	nums.emplace_back(-1,3);
	nums.emplace_back(-3,9);

	puts("--- comparison test ---");
	for (rvec::size_type n = 0; n < nums.size(); ++n) {
		for (rvec::size_type i = n + 1; i < nums.size(); ++i) {
			cout << nums[n] << " & " << nums[i]
			     << "\n    == " << (nums[n] == nums[i] ? "true" : "")
			     << "\n    != " << (nums[n] != nums[i] ? "true" : "")
			     << "\n    <  " << (nums[n] <  nums[i] ? "true" : "")
			     << "\n    >  " << (nums[n] >  nums[i] ? "true" : "")
			     << "\n    <= " << (nums[n] <= nums[i] ? "true" : "")
			     << "\n    >= " << (nums[n] >= nums[i] ? "true" : "") << '\n';
		}
	}
	putchar('\n');
	fflush(stdin);
}


int main() {
	auto begin = clock();

	arithOpTest();
	compOpTest();

	auto end = clock();
	printf(">> TEST FINISHED in %lf ms.\n", ((static_cast<double>(end - begin) / CLOCKS_PER_SEC) * 1000));

	return 0;
}



#include "ggconfig.hpp"
#include <iostream>

using namespace std;


/*
TODO:
- ERROR CHECK number parsing
- proper error system: e.g. from exceptions in number parsing
*/


void run() {
	const char* filename = "test.ggconfig";
	gg::ConfigStorage p;

	if (p.parseFile(filename)) {
		puts("--- Output: ---");
		for (const auto& pair: p.storage) {
			cout << "\033[0;36m" << pair.first << "\033[0m = ";
			switch (pair.second.which()) {
			case 0: // string
				cout << pair.second << '\n';
				break;
			case 1: // double
				cout << "\033[1;36m" << pair.second << "\033[0m\n";
				break;
			case 2: // bool
				cout << "\033[1;35m" << (boost::get<bool>(pair.second) ? "true" : "false") << "\033[0m\n";
				break;
			}
		}
	}

	p["id"] = string("changed");
	cout << "\nid:        " << p["id"]
	     << "\nnot_exist: " << p["not_exist"] << endl;
}


void drawColors() {
	cout << "\033[7;30msome text here\033[0m  " << "\033[1;30msome text here\033[0m  " << "\033[4;30msome text here\033[0m\n"
	     << "\033[7;31msome text here\033[0m  " << "\033[1;31msome text here\033[0m  " << "\033[4;31msome text here\033[0m\n"
	     << "\033[7;32msome text here\033[0m  " << "\033[1;32msome text here\033[0m  " << "\033[4;32msome text here\033[0m\n"
	     << "\033[7;33msome text here\033[0m  " << "\033[1;33msome text here\033[0m  " << "\033[4;33msome text here\033[0m\n"
	     << "\033[7;34msome text here\033[0m  " << "\033[1;34msome text here\033[0m  " << "\033[4;34msome text here\033[0m\n"
	     << "\033[7;35msome text here\033[0m  " << "\033[1;35msome text here\033[0m  " << "\033[4;35msome text here\033[0m\n"
	     << "\033[7;36msome text here\033[0m  " << "\033[1;36msome text here\033[0m  " << "\033[4;36msome text here\033[0m\n"
	     << "\033[7;37msome text here\033[0m  " << "\033[1;37msome text here\033[0m  " << "\033[4;37msome text here\033[0m\n";
}


int main() {
	run();
	// drawColors();
}

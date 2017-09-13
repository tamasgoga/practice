#ifndef GG_ROLL_ROLL_HPP
#define GG_ROLL_ROLL_HPP


//---------------------------------------------------
// class: Roll
//---------------------------------------------------


#include "Die.hpp"

#include <string>
#include <vector>


/* Defines a roll of any number of dice */
class Roll {
public:
	/* Size information about the underlying container */
	using size_type = std::vector<Die>::size_type;

	/* Contains the dices */
	std::vector<Die> dice;

	/* C-tors:
	   1: From a command string, e.g. cmd = {"3d6", "3d6 2d12"}
	      Note that "3d6" will create 3 x Die objects
	   2: Copy c-tor
	**/
	Roll(const std::vector<std::string>& commands);
	Roll(const Roll& other);

	/* D-tor */
	virtual ~Roll() {}

	/* Roll all the dices */
	virtual void roll() {
		for (auto& d: dice)
			d.roll();
	}
};


#endif // GG_ROLL_ROLL_HPP
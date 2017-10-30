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

	/* C-tor:
	     From a command string, e.g. cmd = {"3d6", "3d6 2d12"}
	     Note that "3d6" will create 3 x Die objects
	**/
	Roll(const std::vector<std::string>& commands);

	/* Default copy & move */
	Roll(const Roll& other) = default;
	Roll& operator=(const Roll&) = default;	
	Roll(Roll&& other) = default;
	Roll& operator=(Roll&&) = default;

	/* D-tor */
	virtual ~Roll() {}

	/* Roll all the dices */
	virtual void roll() {
		for (auto& d: dice)
			d.roll();
	}

	/* Output a Roll into a stream */
	friend std::ostream& operator<<(std::ostream& out, const Roll& d);
};


#endif // GG_ROLL_ROLL_HPP
